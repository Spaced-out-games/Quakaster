/// <summary>
/// gfx/types_GL.inl
/// Purpose: Defines type to attribute mappings for OpenGL
/// </summary>

namespace qk::gfx {

    template<>
    inline constexpr bool VertexTypeImplemented<int> = true;

    template<>
    inline constexpr bool VertexTypeImplemented<float> = true;

    template<>
    inline constexpr bool VertexTypeImplemented<unsigned int> = true;

    template<>
    inline constexpr bool VertexTypeImplemented<unsigned char> = true;

    template<>
    inline VertexAttribute VertexAttribute_cast<int>()
    {
        return 0x1404; // GL_INT
    }

    template<>
    inline VertexAttribute VertexAttribute_cast<float>()
    {
        return 0x1406; // GL_FLOAT
    }

    template<>
    inline VertexAttribute VertexAttribute_cast<unsigned int>()
    {
        return 0x1405;//GL_UNSIGNED_INT
    }

    template<>
    inline VertexAttribute VertexAttribute_cast<unsigned char>()
    {
        return 0x1401; //GL_UNSIGNED_BYTE
    }
}