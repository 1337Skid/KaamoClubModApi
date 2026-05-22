#ifndef ASSET_H
#define ASSET_H

class Asset {
    private:
        static inline Globals_Canvas* globals_canvas = 0;
        static inline int next_texture_id = 333701; // we are going to inject our textures there so it's really far from the real texture id
        static inline int next_material_id = 233701; // same thing for this
        static inline int next_mesh_id = 133701; // and this
    public:
        static void init(void);
        static std::string getassetfilepath(unsigned int id);
        static void setassetfilepath(unsigned int id, const std::string value);
        static std::string gettext(int id);
        static int createtexture(const std::string& path);
        static int creatematerial(int diffuse, int normal, int shader);
        static int createmesh(const std::string& path, int material_id);
};
#endif