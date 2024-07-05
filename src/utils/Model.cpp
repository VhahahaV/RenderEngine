//
// Created by czq on 24-6-5.
//

#include "Model.h"
// #define TINYOBJLOADER_IMPLEMENTATION
// #include <fstream>
#include <memory>
#include <tiny_obj_loader.h>
// #define TINYPLY_IMPLEMENTATION
#include <tinyply.h>

#include "FileUtils.h"

uint32_t Model::getShapeCount() const
{
    return mShapes.size();
}

uint32_t Model::getMaterialCount() const
{
    return mShapes.size();
}

// #include <example-utils.hpp>
// #include "FileUtils.h"
//
// std::unique_ptr<Model> loadByObj(const ModelInfo &modelInfo, bool triangulate)
// {
//     auto filename = findObj(modelInfo.mFilename);
//
//     tinyobj::attrib_t attrib; // 所有的数据放在这里
//     std::vector<tinyobj::shape_t> shapes;
//     std::vector<tinyobj::material_t> materials;
//
//     // 一个shape,表示一个部分,
//     // 其中主要存的是索引坐标 mesh_t类
//     // 放在indices中
//     /*
//     // -1 means not used.
//     typedef struct {
//       int vertex_index;
//       int normal_index;
//       int texcoord_index;
//     } index_t;
//     */
//     std::string warn;
//     std::string err;
//     bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(),
//                                 "", triangulate);
//
//     // 接下里就是从上面的属性中取值了
//     if (!warn.empty())
//     {
//         std::cout << "WARN: " << warn << std::endl;
//     }
//     if (!err.empty())
//     {
//         std::cerr << "ERR: " << err << std::endl;
//     }
//     if (!ret)
//     {
//         printf("Failed to load/parse .obj.\n");
//     }
//
//     // ========================== 将读入的模型数据存入自己定义的数据结构中 ========================
//
//     auto MeshData = std::make_unique<Model>();
//     MeshData->mModelInfo = modelInfo;
//     // 获取顶点的坐标以及法线位置
//     {
//         auto& verPos = attrib.vertices;
//         auto& disPos = MeshData->mVertice;
//         auto& vertNor = attrib.normals;
//         auto& disNor = MeshData->mNormal;
//         auto& vertTex = attrib.texcoords;
//         auto& disTex = MeshData->mTexCoords;
//         // for(int i = 0 ; i < verPos.size() ; i+=3)
//         // {float objX = verPos.at(i + 0);
//         //     float objY = verPos.at(i + 1);
//         //     float objZ = verPos.at(i + 2);
//         //     float blenderX = objX;
//         //     float blenderY = -objZ;
//         //     float blenderZ = objY;
//         //     disPos.emplace_back(blenderX);
//         //     disPos.emplace_back(blenderY);
//         //     disPos.emplace_back(blenderZ);
//         // }
//         for (auto& pos : verPos)
//             disPos.emplace_back(pos);
//         for (auto& nor : vertNor)
//             disNor.emplace_back(nor);
//         for (auto& tex : vertTex)
//             disTex.emplace_back(tex);
//     }
//
//     /// Face 数据
//     {
//         // For each shape 遍历每一个部分
//         for (size_t i = 0; i < shapes.size(); i++)
//         {
//             // 这部分的名称
//             printf("shape[%ld].name = %s\n", static_cast<long>(i),
//                    shapes[i].name.c_str());
//             // 网格的点数
//             printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
//                    static_cast<unsigned long>(shapes[i].mesh.indices.size()));
//             auto numFaces = shapes[i].mesh.num_face_vertices.size();
//             auto& FaceIndex = shapes[i].mesh.indices;
//             printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
//                    static_cast<unsigned long>(numFaces));
//             auto& disIndex = MeshData->mFaceIndex;
//             // disIndex.reserve(FaceIndex.size());
//             for (const auto& idx : FaceIndex)
//                 disIndex.emplace_back(idx.vertex_index);
//         }
//     }
//
//     std::cout << "# Loading Complete #" << std::endl;
//     // 材质数据
//     {
//         for (const auto& mat : materials)
//         {
//             Material material;
//             material.path = modelInfo.mFilename+"/"+mat.name;
//             material.ambient = { mat.ambient[0], mat.ambient[1], mat.ambient[2] };
//             material.diffuse = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] };
//             material.specular = { mat.specular[0], mat.specular[1], mat.specular[2] };
//             material.shininess = mat.shininess;
//             material.texture_diffuse = mat.diffuse_texname;
//             material.texture_specular = mat.specular_texname;
//             material.texture_normal = mat.normal_texname;
//
//             MeshData->mMaterials.push_back(material);
//         }
//     }
//
//     MeshData->debug();
//     return MeshData;
// }
std::unique_ptr<Model> loadByObj(const ModelInfo &modelInfo, bool triangulate)
{
    auto filename = findObj(modelInfo.mFilename);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(),
                                "", triangulate);

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "ERR: " << err << std::endl;
    }
    if (!ret) {
        std::cerr << "Failed to load/parse .obj.\n";
        return nullptr;
    }

    auto MeshData = std::make_unique<Model>();
    MeshData->mModelInfo = modelInfo;

    // 全局顶点、法线、纹理坐标缓存
    // MeshData->mVertices = attrib.vertices;
    // MeshData->mNormals = attrib.normals;
    // MeshData->mTexCoords = attrib.texcoords;

    // 处理材质
    for (const auto& mat : materials)
    {
        Material material;
        material.mName = mat.name;
        material.mPath = modelInfo.mFilename+"/"+mat.diffuse_texname;
        material.ambient = { mat.ambient[0], mat.ambient[1], mat.ambient[2] };
        material.diffuse = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] };
        material.specular = { mat.specular[0], mat.specular[1], mat.specular[2] };
        material.shininess = mat.shininess;
        material.texture_diffuse = mat.diffuse_texname;
        material.texture_specular = mat.specular_texname;
        material.texture_normal = mat.normal_texname;

        MeshData->mMaterials.push_back(material);
    }



    // 将Shape数据存储到Model中
    for (size_t i = 0; i < shapes.size(); i++)
    {
        const auto& shape = shapes[i];
        Shape newShape(shape.name, shape.mesh.material_ids.empty() ? 0 : shape.mesh.material_ids[0]);

        // 提取并存储索引
        size_t index_offset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            int fv = shape.mesh.num_face_vertices[f];
            newShape.mNumFaceVertices.push_back(fv);
            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                // newShape.mIndices.push_back(idx.vertex_index);
                // newShape.mIndices.push_back(index_offset+v);
                newShape.mIndices.push_back(newShape.mVertices.size() / 3); // 使用局部索引
                // 将顶点、法线、纹理坐标添加到Shape中
                if (idx.vertex_index >= 0)
                {
                    newShape.mVertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                    newShape.mVertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                    newShape.mVertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                }
                if (idx.normal_index >= 0)
                {
                    newShape.mNormals.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    newShape.mNormals.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    newShape.mNormals.push_back(attrib.normals[3 * idx.normal_index + 2]);
                }
                if (idx.texcoord_index >= 0)
                {
                    newShape.mTexCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    newShape.mTexCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
            }
            index_offset += fv;
        }

        MeshData->mShapes.push_back(std::move(newShape));
    }
    MeshData->debug();
    return MeshData;
}

std::unique_ptr<Model> loadByPly(const ModelInfo &modelInfo,bool requireNormal, bool requireTexcoord)
{
    return nullptr;
    // auto filename = modelInfo.mFilename.c_str();
    // std::cout << "........................................................................\n";
    // std::cout << "Now Reading: " << filename << std::endl;
    //
    // std::unique_ptr<std::istream> file_stream;
    // std::vector<uint8_t> byte_buffer;
    //
    //
    // // For most files < 1gb, pre-loading the entire file upfront and wrapping it into a
    // // stream is a net win for parsing speed, about 40% faster.
    // {
    //     byte_buffer = read_file_binary(filename);
    //     file_stream = std::make_unique<memory_stream>((char*)byte_buffer.data(), byte_buffer.size());
    // }
    //
    //
    // if (!file_stream || file_stream->fail()) throw std::runtime_error(
    //     "file_stream failed to open " + std::string(filename));
    //
    // // file_stream->seekg(0, std::ios::end);
    // // const float size_mb = file_stream->tellg() * float(1e-6);
    // // file_stream->seekg(0, std::ios::beg);
    //
    // tinyply::PlyFile file;
    // file.parse_header(*file_stream);
    //
    // // std::cout << "\t[ply_header] Type: " << (file.is_binary_file() ? "binary" : "ascii") << std::endl;
    // // for (const auto& c : file.get_comments()) std::cout << "\t[ply_header] Comment: " << c << std::endl;
    // // for (const auto& c : file.get_info()) std::cout << "\t[ply_header] Info: " << c << std::endl;
    // //
    // // for (const auto& e : file.get_elements())
    // // {
    // //     std::cout << "\t[ply_header] element: " << e.name << " (" << e.size << ")" << std::endl;
    // //     for (const auto& p : e.properties)
    // //     {
    // //         std::cout << "\t[ply_header] \tproperty: " << p.name << " (type=" << tinyply::PropertyTable[p.propertyType].
    // //             str << ")";
    // //         if (p.isList) std::cout << " (list_type=" << tinyply::PropertyTable[p.listType].str << ")";
    // //         std::cout << std::endl;
    // //     }
    // // }
    //
    // std::shared_ptr<tinyply::PlyData> vertices, normals, texcoords, faces;
    //
    // vertices = file.request_properties_from_element("vertex", {"x", "y", "z"});
    // if(requireNormal)
    //     normals = file.request_properties_from_element("vertex", {"nx", "ny", "nz"});
    // if (requireTexcoord)
    //     texcoords = file.request_properties_from_element("vertex", {"u", "v"});
    // faces = file.request_properties_from_element("face", {"vertex_indices"}, 3);
    // manual_timer read_timer;
    //
    // read_timer.start();
    // file.read(*file_stream);
    // read_timer.stop();
    //
    // // const float parsing_time = static_cast<float>(read_timer.get()) / 1000.f;
    // // std::cout << "\tparsing " << size_mb << "mb in " << parsing_time << " seconds [" << (size_mb / parsing_time) <<
    // //     " MBps]" << std::endl;
    // //
    // // if (vertices) std::cout << "\tRead " << vertices->count << " total vertices " << std::endl;
    // // if (normals) std::cout << "\tRead " << normals->count << " total vertex normals " << std::endl;
    // // if (texcoords) std::cout << "\tRead " << texcoords->count << " total vertex texcoords " << std::endl;
    // // if (faces) std::cout << "\tRead " << faces->count << " total faces (triangles) " << std::endl;
    //
    // auto meshData = std::make_unique<Model>();
    // meshData->mModelInfo = modelInfo;
    // // Example One: converting to your own application types
    // {
    //     // vertices
    //     const size_t numVerticesBytes = vertices->buffer.size_bytes();
    //     auto& disVert = meshData->mVertice;
    //     disVert.resize(vertices->count * 3);
    //     std::memcpy(disVert.data(), vertices->buffer.get(), numVerticesBytes);
    //
    //     // normals
    //     if(requireNormal)
    //     {
    //         const size_t numNormalsBytes = normals->buffer.size_bytes();
    //         auto& disNor = meshData->mNormal;
    //         disNor.resize(normals->count * 3);
    //         std::memcpy(disNor.data(), normals->buffer.get(), numNormalsBytes);
    //     }
    //
    //
    //     // texcoords
    //     if(requireTexcoord)
    //     {
    //         const size_t numTexcBytes = texcoords->buffer.size_bytes();
    //         auto& disTex = meshData->mTexCoords;
    //         disTex.resize(texcoords->count * 2);
    //         std::memcpy(disTex.data(), texcoords->buffer.get(), numTexcBytes);
    //     }
    //
    //     // faces
    //     size_t numFaceBytes = faces->buffer.size_bytes();
    //     if (!numFaceBytes)
    //         throw std::runtime_error("no face int the ply file !!");
    //     auto& disFace = meshData->mFaceIndex;
    //     disFace.resize(faces->count * 3);
    //     std::memcpy(disFace.data(), faces->buffer.get(), numFaceBytes);
    // }
    //
    // meshData->debug();
    // return meshData;
}
