//
// Created by czq on 24-6-5.
//

#include "Mesh.h"
// #define TINYOBJLOADER_IMPLEMENTATION
#include <fstream>
#include <memory>
#include "tiny_obj_loader.h"
// #define TINYPLY_IMPLEMENTATION
#include "tinyply.h"
#include "example-utils.hpp"

std::unique_ptr<Mesh> loadByObj(const char* filename, const char* basepath, bool triangulate)
{
    tinyobj::attrib_t attrib; // 所有的数据放在这里
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    // 一个shape,表示一个部分,
    // 其中主要存的是索引坐标 mesh_t类,
    // 放在indices中
    /*
    // -1 means not used.
    typedef struct {
      int vertex_index;
      int normal_index;
      int texcoord_index;
    } index_t;
    */
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename,
                                basepath, triangulate);

    // 接下里就是从上面的属性中取值了
    if (!warn.empty())
    {
        std::cout << "WARN: " << warn << std::endl;
    }
    if (!err.empty())
    {
        std::cerr << "ERR: " << err << std::endl;
    }
    if (!ret)
    {
        printf("Failed to load/parse .obj.\n");
    }

    // ========================== 将读入的模型数据存入自己定义的数据结构中 ========================

    auto MeshData = std::make_unique<Mesh>();
    // 获取顶点的坐标以及法线位置
    {
        auto& verPos = attrib.vertices;
        auto& disPos = MeshData->mVertice;
        auto& vertNor = attrib.normals;
        auto& disNor = MeshData->mNormal;
        auto& vertTex = attrib.texcoords;
        auto& disTex = MeshData->mTexCoords;
        for (auto& pos : verPos)
            disPos.emplace_back(pos);
        for (auto& nor : vertNor)
            disNor.emplace_back(nor);
        for (auto& tex : vertTex)
            disTex.emplace_back(tex);
    }

    /// Face 数据
    {
        // For each shape 遍历每一个部分
        for (size_t i = 0; i < shapes.size(); i++)
        {
            // 这部分的名称
            printf("shape[%ld].name = %s\n", static_cast<long>(i),
                   shapes[i].name.c_str());
            // 网格的点数
            printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
                   static_cast<unsigned long>(shapes[i].mesh.indices.size()));


            auto numFaces = shapes[i].mesh.num_face_vertices.size();
            auto& FaceIndex = shapes[i].mesh.indices;
            printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
                   static_cast<unsigned long>(numFaces));
            auto& disIndex = MeshData->mFaceIndex;
            disIndex.reserve(FaceIndex.size());
            for (const auto& idx : FaceIndex)
                disIndex.emplace_back(idx.vertex_index);
        }
    }

    std::cout << "# Loading Complete #" << std::endl;
    MeshData->debug();
    return MeshData;
}

std::unique_ptr<Mesh> loadByPly(const char* filename,bool requireNormal, bool requireTexcoord)
{
    std::cout << "........................................................................\n";
    std::cout << "Now Reading: " << filename << std::endl;

    std::unique_ptr<std::istream> file_stream;
    std::vector<uint8_t> byte_buffer;


    // For most files < 1gb, pre-loading the entire file upfront and wrapping it into a
    // stream is a net win for parsing speed, about 40% faster.
    {
        byte_buffer = read_file_binary(filename);
        file_stream = std::make_unique<memory_stream>((char*)byte_buffer.data(), byte_buffer.size());
    }


    if (!file_stream || file_stream->fail()) throw std::runtime_error(
        "file_stream failed to open " + std::string(filename));

    file_stream->seekg(0, std::ios::end);
    const float size_mb = file_stream->tellg() * float(1e-6);
    file_stream->seekg(0, std::ios::beg);

    tinyply::PlyFile file;
    file.parse_header(*file_stream);

    // std::cout << "\t[ply_header] Type: " << (file.is_binary_file() ? "binary" : "ascii") << std::endl;
    // for (const auto& c : file.get_comments()) std::cout << "\t[ply_header] Comment: " << c << std::endl;
    // for (const auto& c : file.get_info()) std::cout << "\t[ply_header] Info: " << c << std::endl;
    //
    // for (const auto& e : file.get_elements())
    // {
    //     std::cout << "\t[ply_header] element: " << e.name << " (" << e.size << ")" << std::endl;
    //     for (const auto& p : e.properties)
    //     {
    //         std::cout << "\t[ply_header] \tproperty: " << p.name << " (type=" << tinyply::PropertyTable[p.propertyType].
    //             str << ")";
    //         if (p.isList) std::cout << " (list_type=" << tinyply::PropertyTable[p.listType].str << ")";
    //         std::cout << std::endl;
    //     }
    // }

    std::shared_ptr<tinyply::PlyData> vertices, normals, texcoords, faces;

    vertices = file.request_properties_from_element("vertex", {"x", "y", "z"});
    if(requireNormal)
        normals = file.request_properties_from_element("vertex", {"nx", "ny", "nz"});
    if (requireTexcoord)
        texcoords = file.request_properties_from_element("vertex", {"u", "v"});
    faces = file.request_properties_from_element("face", {"vertex_indices"}, 3);
    manual_timer read_timer;

    read_timer.start();
    file.read(*file_stream);
    read_timer.stop();

    // const float parsing_time = static_cast<float>(read_timer.get()) / 1000.f;
    // std::cout << "\tparsing " << size_mb << "mb in " << parsing_time << " seconds [" << (size_mb / parsing_time) <<
    //     " MBps]" << std::endl;
    //
    // if (vertices) std::cout << "\tRead " << vertices->count << " total vertices " << std::endl;
    // if (normals) std::cout << "\tRead " << normals->count << " total vertex normals " << std::endl;
    // if (texcoords) std::cout << "\tRead " << texcoords->count << " total vertex texcoords " << std::endl;
    // if (faces) std::cout << "\tRead " << faces->count << " total faces (triangles) " << std::endl;

    auto meshData = std::make_unique<Mesh>();
    // Example One: converting to your own application types
    {
        // vertices
        const size_t numVerticesBytes = vertices->buffer.size_bytes();
        auto& disVert = meshData->mVertice;
        disVert.resize(vertices->count * 3);
        std::memcpy(disVert.data(), vertices->buffer.get(), numVerticesBytes);

        // normals
        if(requireNormal)
        {
            const size_t numNormalsBytes = normals->buffer.size_bytes();
            auto& disNor = meshData->mNormal;
            disNor.resize(normals->count * 3);
            std::memcpy(disNor.data(), normals->buffer.get(), numNormalsBytes);
        }


        // texcoords
        if(requireTexcoord)
        {
            const size_t numTexcBytes = texcoords->buffer.size_bytes();
            auto& disTex = meshData->mTexCoords;
            disTex.resize(texcoords->count * 2);
            std::memcpy(disTex.data(), texcoords->buffer.get(), numTexcBytes);
        }

        // faces
        size_t numFaceBytes = faces->buffer.size_bytes();
        if (!numFaceBytes)
            throw std::runtime_error("no face int the ply file !!");
        auto& disFace = meshData->mFaceIndex;
        disFace.resize(faces->count * 3);
        std::memcpy(disFace.data(), faces->buffer.get(), numFaceBytes);
    }

    meshData->debug();
    return meshData;
}
