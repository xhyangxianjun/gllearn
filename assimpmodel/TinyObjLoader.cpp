#include "TinyObjLoader.h"
#include "ModelShader.h"
#include "ModelUtil.h"
#include "tiny_obj_loader.h"
#include <opencv2/opencv.hpp>

/**
 * Class constructor, loads shaders & gets locations of variables in them
 */
TinyObjLoader::TinyObjLoader() {
    isObjectLoaded = false;

    // shader related setup -- loading, attribute and uniform locations
    std::string vertexShader    = "shaders/modelTextured.vsh";
    std::string fragmentShader  = "shaders/modelTextured.fsh";
    shaderProgramID         = LoadShaders(vertexShader, fragmentShader);
    vertexAttribute         = GetAttributeLocation(shaderProgramID, "vertexPosition");
    vertexUVAttribute       = GetAttributeLocation(shaderProgramID, "vertexUV");
    mvpLocation             = GetUniformLocation(shaderProgramID, "mvpMat");
    textureSamplerLocation  = GetUniformLocation(shaderProgramID, "textureSampler");

    CheckGLError("TinyObjLoader::TinyObjLoader");
}

/**
 * Class destructor, deletes Assimp importer pointer and removes 3D model from GL
 */
TinyObjLoader::~TinyObjLoader() {
    Delete3DModel();
}

/**
 * Generate buffers for vertex positions, texture coordinates, faces -- and load data into them
 */
void TinyObjLoader::GenerateGLBuffers() {

    struct MeshInfo newMeshInfo; // this struct is updated for each mesh in the model
    GLuint buffer;

    // For every mesh -- load face indices, vertex positions, vertex texture coords
    // also copy texture index for mesh into newMeshInfo.textureIndex
    for (unsigned int n = 0; n < shapes.size(); ++n)
    {

        auto mesh = shapes[n].mesh;

        int vertexCount = mesh.indices.size();
        newMeshInfo.numberOfFaces = vertexCount / 3;
        unsigned int *indexData = new unsigned int[vertexCount];
        float *vertexData = new float[vertexCount * 3];
        float *textureCoords = new float[vertexCount * 2];

        unsigned int verIndex = 0;
        unsigned int texIndex = 0;

        for (unsigned int t = 0; t < vertexCount; ++t)
        {
            tinyobj::index_t idx = mesh.indices[t];
            indexData[t] = idx.vertex_index;

//            printf("index = %d, texcoord = %d,  ", idx.vertex_index, idx.texcoord_index);

            float *pverdata = vertexData + verIndex;
            pverdata[0] = attrib.vertices[ idx.vertex_index * 3 + 0];
            pverdata[1] = attrib.vertices[ idx.vertex_index * 3 + 1];
            pverdata[2] = attrib.vertices[ idx.vertex_index * 3 + 2];
            verIndex += 3;
//            printf("vertex[%d], %06f, %06f, %06f,   ", t,  pverdata[0], pverdata[1], pverdata[2]);

            float *ptexdata = textureCoords + texIndex;
            ptexdata[0] = attrib.texcoords[idx.texcoord_index * 2 + 0];
            ptexdata[1] = attrib.texcoords[idx.texcoord_index * 2 + 1];
            texIndex += 2;
//            printf("%06f, %06f \n", ptexdata[0], ptexdata[1] );

        }

        // buffer for faces
        if (newMeshInfo.numberOfFaces)
        {

            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(unsigned int) * newMeshInfo.numberOfFaces * 3, indexData,
                         GL_STATIC_DRAW);
            newMeshInfo.faceBuffer = buffer;

            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(unsigned int) * newMeshInfo.numberOfFaces * 3 * 3, vertexData,
                         GL_STATIC_DRAW);
            newMeshInfo.vertexBuffer = buffer;


            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(float) * newMeshInfo.numberOfFaces * 3 * 2, textureCoords,
                         GL_STATIC_DRAW);
            newMeshInfo.textureCoordBuffer = buffer;

        }
        delete[] indexData;
        delete[] vertexData;
        delete[] textureCoords;

        // buffer for vertex positions
        if (attrib.vertices.size() > 0 )
        {
            int vtxindex = 0;
            for( int i = 0; i < attrib.vertices.size() / 3; ++ i )
            {
//                printf("vertices[%d], %f/%f/%f \n", i, attrib.vertices[vtxindex + 0], attrib.vertices[vtxindex + 1], attrib.vertices[vtxindex + 2]);
                vtxindex += 3;
            }

        }

        // buffer for vertex texture coordinates
        // ***ASSUMPTION*** -- handle only one texture for each mesh
        if (attrib.texcoords.size() > 0 ) {
            int texindex = 0;
            for (int i = 0; i < attrib.vertices.size() / 2; ++i) {
//                printf("vertices[%d], %f/%f \n", i, attrib.vertices[texindex + 0], attrib.vertices[texindex + 1]);
                texindex += 2;
            }

        }


        // unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        auto mid = shapes[n].mesh.material_ids;
        auto materi =  materials[0].diffuse_texname;

        if( !materi.empty() )
        {
            unsigned int textureId = textureNameMap[materi];
            newMeshInfo.textureIndex = textureId;
        } else {
            newMeshInfo.textureIndex = 0;
        }

        modelMeshes.push_back(newMeshInfo);
    }

}


/**
 * Read textures associated with all materials and load images to GL
 */
bool TinyObjLoader::LoadTexturesToGL(std::string modelFilename) {

    // read names of textures associated with all materials
    textureNameMap.clear();

    for (unsigned int m = 0; m < materials.size(); ++m)
    {
        textureNameMap.insert(std::pair<std::string, GLuint>(materials[m].diffuse_texname, 0));
    }

    int numTextures = (int) textureNameMap.size();
    printf("Total number of textures is %d \n", numTextures);

    // create and fill array with texture names in GL
    GLuint * textureGLNames = new GLuint[numTextures];
    glGenTextures(numTextures, textureGLNames);

    // Extract the directory part from the file name
    // will be used to read the texture
    std::string modelDirectoryName = GetDirectoryName(modelFilename);

    // iterate over the textures, read them using OpenCV, load into GL
    std::map<std::string, GLuint>::iterator textureIterator = textureNameMap.begin();
    int i = 0;
    for (; textureIterator != textureNameMap.end(); ++i, ++textureIterator) {

        std::string textureFilename = (*textureIterator).first;  // get filename
        std::string textureFullPath = modelDirectoryName + "/" + textureFilename;
        (*textureIterator).second = textureGLNames[i];	  // save texture id for filename in map

        // load the texture using OpenCV
        printf("Loading texture %s\n", textureFullPath.c_str());
        cv::Mat textureImage = cv::imread(textureFullPath);
        if (!textureImage.empty()) {

            // opencv reads textures in BGR format, change to RGB for GL
            cv::cvtColor(textureImage, textureImage, CV_BGR2RGB);
            // opencv reads image from top-left, while GL expects it from bottom-left
            // vertically flip the image
            cv::flip(textureImage, textureImage, 0);

            // bind the texture
            glBindTexture(GL_TEXTURE_2D, textureGLNames[i]);
            // specify linear filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // load the OpenCV Mat into GLES
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.cols,
                         textureImage.rows, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         textureImage.data);
            CheckGLError("TinyObjLoader::loadGLTexGen");

        } else {

            printf("Couldn't load texture %s\n", textureFilename.c_str());

            //Cleanup and return
            delete[] textureGLNames;
            return false;

        }
    }

    //Cleanup and return
    delete[] textureGLNames;
    return true;
}

/**
 * Loads a general OBJ with many meshes -- assumes texture is associated with each mesh
 * does not handle material properties (like diffuse, specular, etc.)
 */
bool TinyObjLoader::Load3DModel(std::string modelFilename) {

    printf("Scene will be imported now\n");

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelFilename.c_str(),
                                "./", false);

    if( !ret )
    {
        printf("Failed to load/parse .obj.\n");
        return false;
    }

    PrintInfo();

    if(!LoadTexturesToGL(modelFilename)) {
        printf("Unable to load textures\n");
        return false;
    }

    GenerateGLBuffers();

    isObjectLoaded = true;
    return true;
}

bool TinyObjLoader::Export3DModel(std::string modelFilename)
{
//    exporterPtr->Export(scene, "obj", modelFilename);
}

/**
 * Clears memory associated with the 3D model
 */
void TinyObjLoader::Delete3DModel() {
    if (isObjectLoaded) {
        // clear modelMeshes stuff
//        for (unsigned int i = 0; i < modelMeshes.size(); ++i) {
//            glDeleteTextures(1, &(modelMeshes[i].textureIndex));
//        }
        modelMeshes.clear();

        printf("Deleted Assimp object\n");
        isObjectLoaded = false;
    }
}

/**
 * Renders the 3D model by rendering every mesh in the object
 */
void TinyObjLoader::Render3DModel(glm::mat4 *mvpMat) {

    if (!isObjectLoaded) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat *) mvpMat);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textureSamplerLocation, 0);

    unsigned int numberOfLoadedMeshes = modelMeshes.size();

    // render all meshes
    for (unsigned int n = 0; n < numberOfLoadedMeshes; ++n) {

        // Texture
        if (modelMeshes[n].textureIndex) {
            glBindTexture( GL_TEXTURE_2D, modelMeshes[n].textureIndex);
        }

        // Faces
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelMeshes[n].faceBuffer);

        // Vertices
        glBindBuffer(GL_ARRAY_BUFFER, modelMeshes[n].vertexBuffer);
        glEnableVertexAttribArray(vertexAttribute);
        glVertexAttribPointer(vertexAttribute, 3, GL_FLOAT, 0, 0, 0);

        // Texture coords
        glBindBuffer(GL_ARRAY_BUFFER, modelMeshes[n].textureCoordBuffer);
        glEnableVertexAttribArray(vertexUVAttribute);
        glVertexAttribPointer(vertexUVAttribute, 2, GL_FLOAT, 0, 0, 0);

//        glDrawElements(GL_TRIANGLES, modelMeshes[n].numberOfFaces * 3, GL_UNSIGNED_INT, 0);

        glDrawArrays(GL_TRIANGLES, 0, modelMeshes[n].numberOfFaces * 9);

        // unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

    CheckGLError("TinyObjLoader::renderObject() \n");

}

void TinyObjLoader::PrintInfo()
{
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
    {
        printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.vertices[3 * v + 0]),
               static_cast<const double>(attrib.vertices[3 * v + 1]),
               static_cast<const double>(attrib.vertices[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.normals.size() / 3; v++)
    {
        printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.normals[3 * v + 0]),
               static_cast<const double>(attrib.normals[3 * v + 1]),
               static_cast<const double>(attrib.normals[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.texcoords.size() / 2; v++)
    {
        printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.texcoords[2 * v + 0]),
               static_cast<const double>(attrib.texcoords[2 * v + 1]));
    }

    // For each shape
    for (size_t i = 0; i < shapes.size(); i++)
    {
        printf("shape[%ld].name = %s\n", static_cast<long>(i),
               shapes[i].name.c_str());
        printf("Size of shape[%ld].indices: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.indices.size()));

        size_t index_offset = 0;

        assert(shapes[i].mesh.num_face_vertices.size() ==
               shapes[i].mesh.material_ids.size());

        printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

        // For each face
        for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
            size_t fnum = shapes[i].mesh.num_face_vertices[f];

            printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
                   static_cast<unsigned long>(fnum));

            // For each vertex in the face
            for (size_t v = 0; v < fnum; v++) {
                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
                printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
                       static_cast<long>(v), idx.vertex_index, idx.normal_index,
                       idx.texcoord_index);
            }

            printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
                   shapes[i].mesh.material_ids[f]);

            index_offset += fnum;
        }

        printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.tags.size()));
        for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
            printf("  tag[%ld] = %s ", static_cast<long>(t),
                   shapes[i].mesh.tags[t].name.c_str());
            printf(" ints: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
                printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
                if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");

            printf(" floats: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
                printf("%f", static_cast<const double>(
                        shapes[i].mesh.tags[t].floatValues[j]));
                if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");

            printf(" strings: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
                printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
                if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");
            printf("\n");
        }
    }

    for (size_t i = 0; i < materials.size(); i++) {
        printf("material[%ld].name = %s\n", static_cast<long>(i),
               materials[i].name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].ambient[0]),
               static_cast<const double>(materials[i].ambient[1]),
               static_cast<const double>(materials[i].ambient[2]));
        printf("  material.Kd = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].diffuse[0]),
               static_cast<const double>(materials[i].diffuse[1]),
               static_cast<const double>(materials[i].diffuse[2]));
        printf("  material.Ks = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].specular[0]),
               static_cast<const double>(materials[i].specular[1]),
               static_cast<const double>(materials[i].specular[2]));
        printf("  material.Tr = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].transmittance[0]),
               static_cast<const double>(materials[i].transmittance[1]),
               static_cast<const double>(materials[i].transmittance[2]));
        printf("  material.Ke = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].emission[0]),
               static_cast<const double>(materials[i].emission[1]),
               static_cast<const double>(materials[i].emission[2]));
        printf("  material.Ns = %f\n",
               static_cast<const double>(materials[i].shininess));
        printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
        printf("  material.dissolve = %f\n",
               static_cast<const double>(materials[i].dissolve));
        printf("  material.illum = %d\n", materials[i].illum);
        printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
        printf("  material.map_Ns = %s\n",
               materials[i].specular_highlight_texname.c_str());
        printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
        printf("    bump_multiplier = %f\n", static_cast<const double>(materials[i].bump_texopt.bump_multiplier));
        printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
        printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
        printf("  <<PBR>>\n");
        printf("  material.Pr     = %f\n", static_cast<const double>(materials[i].roughness));
        printf("  material.Pm     = %f\n", static_cast<const double>(materials[i].metallic));
        printf("  material.Ps     = %f\n", static_cast<const double>(materials[i].sheen));
        printf("  material.Pc     = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
        printf("  material.Pcr    = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
        printf("  material.aniso  = %f\n", static_cast<const double>(materials[i].anisotropy));
        printf("  material.anisor = %f\n", static_cast<const double>(materials[i].anisotropy_rotation));
        printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
        printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
        printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
        printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
        printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
        std::map<std::string, std::string>::const_iterator it(
                materials[i].unknown_parameter.begin());
        std::map<std::string, std::string>::const_iterator itEnd(
                materials[i].unknown_parameter.end());

        for (; it != itEnd; it++) {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }
}