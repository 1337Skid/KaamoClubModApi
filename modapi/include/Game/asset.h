#ifndef ASSET_H
#define ASSET_H

class Asset {
    private:
        struct EarlyAssets {
            int id;
            std::string path;
            int textureid;
            int regionid;
        };
        static inline Globals_Canvas *globals_canvas = 0;
    public:
        static inline int next_texture_id = 333701; // we are going to inject our textures there so it's really far from the real texture id
        static inline int next_material_id = 233701; // same thing for this
        static inline int next_mesh_id = 133701; // and this
        static inline int next_sprite_id = 9872; // and this !!!
        static inline std::vector<EarlyAssets> created_earlyassets;
        static void init(void);
        static std::string getassetfilepath(unsigned int id);
        static void setassetfilepath(unsigned int id, const std::string value);
        static std::string gettext(int id);
        static int createtexture(const std::string& path);
        static int creatematerial(int diffuse, int normal, int shader);
        static int createmesh(const std::string& path, int material_id);
        static void drawstring(const std::string& text, int x, int y);
        static void setcolor(int r, int g, int b, int a);
        static void drawimage2d(unsigned int id, int x, int y);
        static int image2dcreate(uint16_t id);
        static int createsprite(int textureid, int regionid);
        static void fmodsound_play(int attachsound, unsigned int a3);
        static int gamecreatetexture(uint16_t id, float a4 = 0.0f);
};
#endif