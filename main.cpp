#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL2_gfx.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include<SDL_mixer.h>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

//screen width and height
const int width=1200, height=750;

//fps vars
double fps=60;
double desiredDelta=1000.0/fps;
Uint32 elapsedTicks=0, startTicks=0;

bool test = true;

//mouse vars
bool l_press = false;
int m_x=0, m_y=0;
int statX=0, statY=0;

//keyboard vars
bool up=false,down=false,t_right=false,t_left=false;

//target ball vars
struct target_ball{
    int rad=24, n=17;
    char color1='\0',color2='\0';
    double x=0, y=0;
    bool    visit=false,
            traverse=false,
            show=false,
            falling=false,
            all=false,
            stone=false,
            chain = false,
            slow = false,
            stop = false;
} tg_ball;
vector<vector<target_ball>> tg_vector;

//shoot ball vars
struct shoot_ball{
    int rad=24;
    char color1='\0', color2='\0';
    double x=double(width)/2, y=height-rad/2-41;
    bool all=false,
         bomb=false,
         launch = false;
} sh_ball;
vector<shoot_ball> sh_vector;
double speed=18, x_speed=0, y_speed=0;

//collision detection vars
struct collision{
    int i=0,j=0;
    bool color=false;
} col_ball;
vector<collision> collide;

//sh_vector recolor holder
struct color_holder{
    char r='k',
         g='k',
         b='k',
         p='k',
         y='k',
         o='k';
} holder;

//db stuff
struct record_db{
    string username,password;
    long long norm=0,inf=0,timer=0,hard=0;
    long long magic1=0,magic2=0,magic3=0,magic4=0;
    double sound=0, chunk=0;
    long long theme=0, back=0, ball=0, cannon=0, music=0;
    int y=0;
} record;
vector<record_db> record_vec;//returns db file data

//game_textures and recs
SDL_Texture* wall_texture;
SDL_Rect wall_rect;
SDL_Texture* wire;
SDL_Rect wire_rect;
SDL_Texture* cannon;
SDL_Rect cannon_rect;
SDL_Point center;
SDL_Texture* bullet;
SDL_Rect bullet_rect;
SDL_Rect magic_rect1;
SDL_Rect magic_rect2;
SDL_Rect magic_rect3;
SDL_Rect magic_rect4;

SDL_Texture* frame1;
SDL_Texture* frame2;
SDL_Texture* frame3;
SDL_Texture* frame4;
SDL_Texture* frame5;
SDL_Texture* frame6;
SDL_Texture* frame7;
SDL_Texture* frame8;
SDL_Texture* frame9;

SDL_Texture* fr1;
SDL_Texture* fr2;
SDL_Texture* fr3;
SDL_Texture* fr4;
SDL_Texture* fr5;
SDL_Texture* fr6;
SDL_Texture* fr7;
SDL_Texture* fr8;
SDL_Texture* fr9;
SDL_Rect exp_rect;

SDL_Texture* fram1;
SDL_Texture* fram2;
SDL_Texture* fram3;
SDL_Texture* fram4;
SDL_Rect laser_rect;

int bombX,bombY;
int bomb_frame = -1;
int laser_frame = -1;

SDL_Texture* game_setting_window;
SDL_Rect game_window_rect;
SDL_Texture* game_exit_window;
SDL_Texture* game_exit_yes;
SDL_Rect game_exit_yes_rect;
SDL_Texture* game_defeat;
SDL_Rect game_defeat_rect;
SDL_Texture* game_exit_btn;
SDL_Rect game_exit_rect;
SDL_Rect game_resume_rect;
SDL_Texture* game_setting_btn;
SDL_Rect game_setting_btn_rect;
SDL_Texture* game_music;
string game_music_path = "game/music.png";
SDL_Rect game_music_rect;

SDL_Rect gmusic_toggle_rect;
SDL_Rect gchunk_toggle_rect;
SDL_Rect game_volume_rect;
SDL_Rect game_music_fissure;
SDL_Rect game_chunk_fissure;

//ball texture and recs
SDL_Texture* chain;

SDL_Texture* texture_r;
SDL_Texture* texture_g;
SDL_Texture* texture_b;
SDL_Texture* texture_p;
SDL_Texture* texture_y;
SDL_Texture* texture_o;

SDL_Texture* texture_bg;
SDL_Texture* texture_br;
SDL_Texture* texture_by;
SDL_Texture* texture_bp;
SDL_Texture* texture_bo;
SDL_Texture* texture_gr;
SDL_Texture* texture_gy;
SDL_Texture* texture_gp;
SDL_Texture* texture_go;
SDL_Texture* texture_ry;
SDL_Texture* texture_rp;
SDL_Texture* texture_ro;
SDL_Texture* texture_yp;
SDL_Texture* texture_yo;
SDL_Texture* texture_po;
SDL_Texture* texture_slow;
SDL_Texture* texture_stop;

SDL_Texture* texture_all;
SDL_Texture* texture_laser;
SDL_Texture* texture_stone;
SDL_Texture* texture_bomb;
SDL_Texture* texture_thunder;
SDL_FRect BallRect;

//SDL vars
SDL_Window *gWindow=NULL;
SDL_Renderer *gRenderer=NULL;
SDL_Event event;
TTF_Font* font_rank;
TTF_Font* font_py_hd;
TTF_Font* font_register;
bool quit = false;


bool game_finish = false, win = false, game_exit = false, defeat = false;//game state
bool once = true;//initializer
bool open_register = true, show_register = false, new_player = false, input = false, last = false, error_log = false, error_reg = false;//input handling
bool intro = false, mid = false, pro = false;
bool laser_power = false;
bool thunder_power = false;
bool halt = false;
bool slow_motion = false;
bool stop_motion = false;
long long time_effect;
long long effect_counter;
long long  mode=0, inf_count=0, startTimer=0, wait = -1600, norm_score=0, inf_score=0, time_score=0, hard_score=0;//game wait and scores and stuff
long long temp1=0, temp2=0, temp3=0, temp4=0;//temp scores
long long balls1=0, balls2=0, balls3=0, balls4=0;
long long score_counter=0;
long long colorfulC = 0, bombC = 0, laserC = 0, thunderC;//magic ball count
long long norm_lv = 1, inf_lv = 1, time_lv = 1, hardcore_lv = 1;//level number
long long themeP=0, backgroundP=0, ballP=0, cannonP=0, musicP=0;
double soundP=0.5, chunkP=0.5;
bool frozenBool = false, safariBool = false, pacificBool = false, jungleBool = false;

//mode boolean
bool gHome = true;
bool gSetting = false;
bool gRanking = false;
bool gMenu = false;
bool gNormal = false;
bool gInfinite = false;
bool gTimer = false;
bool gHardcore = false;
bool gPause = false;
bool gEnd = false;

bool theme_sec = false;
bool music_sec = false;
bool canon_sec = false;
bool ball_sec = false;
bool backg_sec = false;
bool theme_choose_sec = false;

//home textures and recs
SDL_Texture* home_bg;
SDL_Rect home_bg_rect;

//socials
SDL_Texture* facebook;
string facebook_path="home/facebook.png";
SDL_Texture* youtube;
string youtube_path="home/youtube.png";
SDL_Texture* instagram;
string instagram_path="home/instagram.png";
SDL_Texture* email;
string email_path="home/email.png";
SDL_Rect facebook_rect;
SDL_Rect youtube_rect;
SDL_Rect instagram_rect;
SDL_Rect email_rect;

//utility
SDL_Texture* rate;
string rate_path="home/rate.png";
SDL_Texture* home_setting;
string home_setting_path="home/home_setting.png";
SDL_Texture* credit;
string credit_path="home/credit.png";
SDL_Rect rate_rect;
SDL_Rect home_setting_rect;
SDL_Rect credit_rect;

//buttons
SDL_Texture* start;
string start_path="home/start.png";
SDL_Texture* ranking;
string ranking_path="home/ranking.png";
SDL_Rect start_rect;
SDL_Rect ranking_rect;

//register
SDL_Texture* exit_btn;
SDL_Rect register_exit_rect;

SDL_Texture* login_btn;
SDL_Texture* login_btnBig;
string login_btn_path="menu/login.png";
SDL_Texture* register_btn;
SDL_Texture* register_btnBig;
string register_btn_path="menu/register.png";
SDL_Rect login_rect;
SDL_Rect register_rect;

SDL_Texture* field_holder;
SDL_Rect user_rect;
SDL_Rect pass_rect;

SDL_Texture* registration;
SDL_Rect register_window;
SDL_Rect error_rect;

SDL_Surface* textSur;
SDL_Texture* textTex;
string user,pass;
string wrong_log="wrong username or password";
string wrong_reg="    username already exists";

//settings
SDL_Texture* chunk_toggle;
SDL_Rect chunk_toggle_rect;
SDL_Rect chunk_fissure_rect;
SDL_Rect volume_rect;

SDL_Texture* music_toggle;
SDL_Rect music_toggle_rect;
SDL_Rect music_fissure_rect;

SDL_Texture* theme;
SDL_Rect theme_rect;
string theme_path = "home/theme.png";
SDL_Texture* music;
SDL_Rect music_rect;
string music_path = "home/music.png";

SDL_Texture* setting_window;
SDL_Texture* theme_btn;
SDL_Rect theme_btn_rect;
string theme_btn_path="home/theme_btn.png";

SDL_Texture* ball_btn;
SDL_Rect ball_btn_rect;
string ball_btn_path="home/ball_btn.png";

SDL_Texture* cannon_btn;
SDL_Rect cannon_btn_rect;
string cannon_btn_path="home/cannon_btn.png";

SDL_Texture* background_btn;
SDL_Rect background_btn_rect;
string background_btn_path="home/background_btn.png";

SDL_Texture* safari;
SDL_Rect safari_rect;
SDL_Rect gsafari_rect;
string safari_path="home/safari.png";

SDL_Texture* jungle;
SDL_Rect jungle_rect;
SDL_Rect gjungle_rect;
string jungle_path="home/jungle.png";

SDL_Texture* frozen;
SDL_Rect frozen_rect;
SDL_Rect gfrozen_rect;
string frozen_path="home/frozen.png";

SDL_Texture* pacific;
SDL_Rect pacific_rect;
SDL_Rect gpacific_rect;
string pacific_path="home/pacific.png";

SDL_Rect setting_exit_rect;
SDL_Texture* settings_return;
SDL_Rect settings_return_rect;

SDL_Texture* wood_canon;
SDL_Rect wood_canon_rect;
SDL_Texture* metal_canon;
SDL_Rect metal_canon_rect;

SDL_Texture* beach_ball;
SDL_Rect beach_ball_rect;
SDL_Texture* reg_ball;
SDL_Rect reg_ball_rect;

SDL_Texture* wood_theme;
SDL_Rect wood_theme_rect;
SDL_Texture* metal_theme;
SDL_Rect metal_theme_rect;

SDL_Texture* war;
SDL_Rect war_rect;
SDL_Texture* peace;
SDL_Rect peace_rect;

//ranking
SDL_Texture* ranking_window;
SDL_Texture* info_holder;
SDL_Texture* header_holder;
SDL_Texture* user_icon;
SDL_Rect ranking_window_rect;
SDL_Rect ranking_exit_rect;
SDL_Rect info_holder_rect;
SDL_Rect header_holder_rect;
SDL_Rect user_icon_rect;
SDL_Rect user_header, norm_header, inf_header, time_header, hard_header, overall_header, rank_header;
SDL_Rect info;

SDL_Texture* gold;
SDL_Texture* silver;
SDL_Texture* bronze;
SDL_Rect medal;

//menu textures and recs
SDL_Texture* menu_bg;
SDL_Rect menu_bg_rect;
SDL_Texture* menu_header;
SDL_Rect  menu_header_rect;
SDL_Texture* player_header;
SDL_Rect  player_header_rect;
SDL_Texture* menu_return;
string menu_return_path="menu/return.png";
SDL_Rect  menu_return_rect;
SDL_Texture* shop;
string shop_path="menu/shop.png";
SDL_Rect  shop_rect;

SDL_Texture* point;
SDL_Texture* pointBig;
SDL_Rect point_rect;

SDL_Texture* normal_mode;
SDL_Texture* infinite_mode;
SDL_Texture* timer_mode;
SDL_Texture* hardcore_mode;
SDL_Rect normal_mode_rect;
SDL_Rect infinite_mode_rect;
SDL_Rect timer_mode_rect;
SDL_Rect hardcore_mode_rect;
SDL_Rect layer_rect;

SDL_Texture* button1;
SDL_Texture* button1Big;
SDL_Rect norm_button1_rect;
SDL_Rect inf_button1_rect;
SDL_Rect time_button1_rect;
SDL_Rect hard_button1_rect;

SDL_Texture* button2;
SDL_Texture* button2Big;
SDL_Rect norm_button2_rect;
SDL_Rect inf_button2_rect;
SDL_Rect time_button2_rect;
SDL_Rect hard_button2_rect;

SDL_Texture* button3;
SDL_Texture* button3Big;
SDL_Rect norm_button3_rect;
SDL_Rect inf_button3_rect;
SDL_Rect time_button3_rect;
SDL_Rect hard_button3_rect;

SDL_Texture* lock1;
SDL_Texture* lock1Big;
SDL_Rect norm_lock1_rect;
SDL_Rect inf_lock1_rect;
SDL_Rect time_lock1_rect;
SDL_Rect hard_lock1_rect;

SDL_Texture* lock2;
SDL_Texture* lock2Big;
SDL_Rect norm_lock2_rect;
SDL_Rect inf_lock2_rect;
SDL_Rect time_lock2_rect;
SDL_Rect hard_lock2_rect;

SDL_Texture* medal1;
SDL_Texture* medal1Big;
SDL_Rect norm_medal1_rect;
SDL_Rect inf_medal1_rect;
SDL_Rect time_medal1_rect;
SDL_Rect hard_medal1_rect;

SDL_Texture* medal2;
SDL_Texture* medal2Big;
SDL_Rect norm_medal2_rect;
SDL_Rect inf_medal2_rect;
SDL_Rect time_medal2_rect;
SDL_Rect hard_medal2_rect;

SDL_Texture* medal3;
SDL_Texture* medal3Big;
SDL_Rect norm_medal3_rect;
SDL_Rect inf_medal3_rect;
SDL_Rect time_medal3_rect;
SDL_Rect hard_medal3_rect;

//end
SDL_Texture* end_bg;
SDL_Texture* end_window;
SDL_Texture* to_menu;
string to_menu_path = "end/to_menu.png";
SDL_Texture* to_home;
string to_home_path = "end/to_home.png";
SDL_Texture* replay;
string replay_path = "end/replay.png";
SDL_Rect end_bg_rect;
SDL_Rect end_window_rect;
SDL_Rect to_menu_rect;
SDL_Rect to_home_rect;
SDL_Rect replay_rect;
SDL_Texture* win_sign;
SDL_Texture* lose_sign;
SDL_Rect result_sign_rect;

//music and chunks
Mix_Music* frozen_music = NULL;
Mix_Music* safari_music = NULL;
Mix_Music* pacific_music = NULL;
Mix_Music* jungle_music = NULL;
Mix_Music* game_win_music = NULL;
Mix_Music* game_lose_music = NULL;
Mix_Music* background_music = NULL;
Mix_Chunk* toggle_chunk = NULL;
Mix_Chunk* bomb_explosion_chunk = NULL;
Mix_Chunk* ball_explosion_chunk = NULL;
Mix_Chunk* laser_chunk = NULL;
Mix_Chunk* thunder_chunk = NULL;

//functions;
bool button(SDL_Rect Rect);
void change_ball_color(bool beach);
void change_theme(bool metal);
void text(SDL_Renderer* renderer, int x, int y, const char* text, int r, int g, int b, int a, int n);
void home_draw(SDL_Renderer* renderer);
void home_focus();
void draw_register(SDL_Renderer* renderer);
int checkCredentials(const string& username, const string& password);
void pull_db();
void text_writer(SDL_Renderer* renderer);
void draw_home_setting(SDL_Renderer* renderer);
void rank_scroll(bool u);
void draw_ranking(SDL_Renderer* renderer);
double square(double a, double b, double c, double d);
void ball_drawer(SDL_Renderer* renderer, char& ball1, char& ball2, bool all, bool stone, bool bomb, bool prisoner, bool slow, bool stop, bool laser, bool thunder);
void remover(vector<vector<target_ball>>& vec1, vector<shoot_ball>& vec2, int i, int j, int& count);
void reInit();
void terminator(vector<vector<target_ball>>& vec1);
bool isCollision(const target_ball &ball1, const target_ball &ball2);
bool isValidMove(int row, int col);
bool findPath(vector<vector<target_ball>>& vec, int row, int col);
void climber(vector<vector<target_ball>>& vec);
void normal_init();
void tg_ball_draw(SDL_Renderer* renderer, bool x, bool y);
void ball_stat();
bool collision(SDL_Renderer* renderer);
void sh_vec_filler();
void sh_vec_draw(SDL_Renderer* renderer, bool x);
void guide(SDL_Renderer* renderer, bool y);
void laserGuide(SDL_Renderer* renderer, bool y);
void laser();
void home(SDL_Renderer* renderer);
void menu_scroll(bool r);
void menu(SDL_Renderer* renderer);
void end(SDL_Renderer* renderer, int k);
void color(char& color1, char& color2);
bool pattern(vector<vector<target_ball>>& vec, int i, int j, int n, char color1, char color2);
bool recolor(vector<vector<target_ball>>& vec, int i, int j, int n);
void normal(SDL_Renderer* renderer);
void infinite_init();
void infinite(SDL_Renderer* renderer);
void timer(SDL_Renderer* renderer);

int main(int argc, char* args[]) {
    srand(time(NULL));

    //SDL initialization
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    gWindow = SDL_CreateWindow("Balls of Steel",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width, height,
                              SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    //font init
    TTF_Init();
    SDL_StartTextInput();
    font_rank = TTF_OpenFont("arial.ttf", 20);
    font_py_hd = TTF_OpenFont("arial.ttf", 25);
    font_register = TTF_OpenFont("arial.ttf", 40);

    //music and chunk init
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    frozen_music = Mix_LoadMUS( "frozen/main_theme.ogg" );
    safari_music = Mix_LoadMUS( "safari/music_main_theme.ogg" );
    jungle_music = Mix_LoadMUS( "jungle/music_main_theme.ogg" );
    pacific_music = Mix_LoadMUS( "pacific/music_main_theme.ogg" );
    game_win_music = Mix_LoadMUS( "frozen/music_game_won.ogg");
    game_lose_music = Mix_LoadMUS( "frozen/music_game_lost.ogg");
    background_music = Mix_LoadMUS( "frozen/atmo_loop.ogg");
    Mix_VolumeMusic(MIX_MAX_VOLUME/5);
    frozenBool = true;

    toggle_chunk = Mix_LoadWAV("frozen/sfx_button_small_1.wav");
    ball_explosion_chunk = Mix_LoadWAV("frozen/explosion_1.wav");
    bomb_explosion_chunk = Mix_LoadWAV("frozen/explosion_2.wav");
    laser_chunk = Mix_LoadWAV("frozen/laser.wav");
    thunder_chunk = Mix_LoadWAV("frozen/thunder.wav");
    Mix_VolumeChunk(toggle_chunk ,MIX_MAX_VOLUME/2);
    Mix_VolumeChunk(ball_explosion_chunk ,MIX_MAX_VOLUME/2);
    Mix_VolumeChunk(bomb_explosion_chunk ,MIX_MAX_VOLUME/2);
    Mix_VolumeChunk(laser_chunk ,MIX_MAX_VOLUME/2);
    Mix_VolumeChunk(thunder_chunk ,MIX_MAX_VOLUME/2);

    wood_canon = IMG_LoadTexture(gRenderer, "home/wood_canon.png");
    metal_canon = IMG_LoadTexture(gRenderer, "home/metal_canon.png");
    wood_canon_rect = {width/2-300/2, height/2+36-100, 300, 80};
    metal_canon_rect = {width/2-300/2, height/2+36*2+80-100, 300, 80};

    beach_ball = IMG_LoadTexture(gRenderer, "home/beach_ball.png");
    beach_ball_rect = {width/2-300/2, height/2+36-100, 300, 80};
    reg_ball = IMG_LoadTexture(gRenderer, "home/reg_ball.png");
    reg_ball_rect = {width/2-300/2, height/2+36*2+80-100, 300, 80};

    war = IMG_LoadTexture(gRenderer, "home/war.png");
    peace = IMG_LoadTexture(gRenderer, "home/peace.png");
    peace_rect = {width/2-300/2, height/2+36-100, 300, 80};
    war_rect = {width/2-300/2, height/2+36*2+80-100, 300, 80};

    wood_theme = IMG_LoadTexture(gRenderer, "home/wood_theme.png");
    metal_theme = IMG_LoadTexture(gRenderer, "home/metal_theme.png");
    wood_theme_rect = {width/2-300/2, height/2+36-100, 300, 80};
    metal_theme_rect = {width/2-300/2, height/2+36*2+80-100, 300, 80};

    //home bg
    home_bg = IMG_LoadTexture(gRenderer, "home/background.png");
    home_bg_rect = {0, 0, width, height};

    //socials
    facebook = IMG_LoadTexture(gRenderer, facebook_path.c_str());
    youtube = IMG_LoadTexture(gRenderer, youtube_path.c_str());
    instagram = IMG_LoadTexture(gRenderer, instagram_path.c_str());
    email = IMG_LoadTexture(gRenderer, email_path.c_str());
    facebook_rect = {40, 34, 65, 65};
    youtube_rect = {40, 134, 65, 65};
    instagram_rect = {40, 234, 65, 65};
    email_rect = {40, 334, 65, 65};

    //utility
    rate = IMG_LoadTexture(gRenderer, rate_path.c_str());
    home_setting = IMG_LoadTexture(gRenderer, home_setting_path.c_str());
    credit = IMG_LoadTexture(gRenderer, credit_path.c_str());
    rate_rect = {1060, 34, 100,50};
    home_setting_rect = {1060, 134, 100,50};
    credit_rect = {1060, 244, 100,50};

    //main home buttons
    start = IMG_LoadTexture(gRenderer, start_path.c_str());
    ranking = IMG_LoadTexture(gRenderer, ranking_path.c_str());
    start_rect = {width/2-330/2, 480, 330, 62};
    ranking_rect = {width/2-330/2, 570, 330, 62};

    registration = IMG_LoadTexture(gRenderer, "home/registration.png");
    register_window = {width/2-500/2, height/2-500/2+25, 500, 500};

    //settings
    chunk_toggle = IMG_LoadTexture(gRenderer, "home/chunk_toggle.png");
    music_toggle = IMG_LoadTexture(gRenderer, "home/music_toggle.png");
    theme = IMG_LoadTexture(gRenderer, "home/theme.png");
    music = IMG_LoadTexture(gRenderer, "home/music.png");
    chunk_toggle_rect = {register_window.x+register_window.w/2-40/2, register_window.y+90, 40, 40};
    chunk_fissure_rect = {register_window.x+register_window.w/2-360/2, register_window.y+80, 360, 60};
    music_toggle_rect = {register_window.x+register_window.w/2-40/2, register_window.y+160, 40, 40};
    music_fissure_rect = {register_window.x+register_window.w/2-360/2, register_window.y+150, 360, 60};
    theme_rect = {register_window.x+register_window.w/2-340/2, register_window.y+244, 340, 80};
    music_rect = {register_window.x+register_window.w/2-340/2, register_window.y+372, 340, 80};

    setting_window = IMG_LoadTexture(gRenderer, "home/setting_window.png");
    theme_btn = IMG_LoadTexture(gRenderer, theme_btn_path.c_str());
    background_btn = IMG_LoadTexture(gRenderer, background_btn_path.c_str());
    ball_btn = IMG_LoadTexture(gRenderer, ball_btn_path.c_str());
    cannon_btn = IMG_LoadTexture(gRenderer, cannon_btn_path.c_str());
    theme_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36, 300, 80};
    background_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*2+80, 300, 80};
    ball_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*3+80*2, 300, 80};
    cannon_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*4+80*3, 300, 80};

    safari = IMG_LoadTexture(gRenderer, safari_path.c_str());
    pacific = IMG_LoadTexture(gRenderer, pacific_path.c_str());
    frozen = IMG_LoadTexture(gRenderer, frozen_path.c_str());
    jungle = IMG_LoadTexture(gRenderer, jungle_path.c_str());
    pacific_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36, 300, 80};
    frozen_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*2+80, 300, 80};
    safari_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*3+80*2, 300, 80};
    jungle_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*4+80*3, 300, 80};

    settings_return = IMG_LoadTexture(gRenderer, "home/return.png");
    settings_return_rect = {register_window.x-80, register_window.y+register_window.h/2-25, 80, 50};
    setting_exit_rect = {register_window.x+register_window.w, register_window.y+register_window.h/2-25,80,50};

    //ranking
    ranking_window = IMG_LoadTexture(gRenderer, "home/ranking_window.png");
    ranking_window_rect = {width/2-900/2, height/2-700/2, 900, 700};
    header_holder = IMG_LoadTexture(gRenderer, "home/header_holder.png");
    header_holder_rect = {ranking_window_rect.x+ranking_window_rect.w/2-870/2, ranking_window_rect.y+50, 870, 60};
    info_holder = IMG_LoadTexture(gRenderer, "home/info_holder.png");
    info_holder_rect = {ranking_window_rect.x+ranking_window_rect.w/2-870/2, 0, 870, 60};
    ranking_exit_rect = {ranking_window_rect.x+ranking_window_rect.w, height/2-25,80,50};

    user_icon = IMG_LoadTexture(gRenderer, "home/user-icon.png");
    user_icon_rect={header_holder_rect.x+115,header_holder_rect.y+header_holder_rect.h/4+2,35,55};
    gold = IMG_LoadTexture(gRenderer, "home/gold.png");
    silver = IMG_LoadTexture(gRenderer, "home/silver.png");
    bronze = IMG_LoadTexture(gRenderer, "home/bronze.png");
    medal = {header_holder_rect.x+42, 0, 42, 90};
    user_header = {header_holder_rect.x+170, header_holder_rect.y+header_holder_rect.h/4+2, 100, 60};
    norm_header = {header_holder_rect.x+270, header_holder_rect.y+header_holder_rect.h/4+2, 100, 60};
    inf_header = {header_holder_rect.x+370, header_holder_rect.y+header_holder_rect.h/4+2, 100, 60};
    time_header = {header_holder_rect.x+470, header_holder_rect.y+header_holder_rect.h/4+2, 100, 60};
    hard_header = {header_holder_rect.x+570-15, header_holder_rect.y+header_holder_rect.h/4+2, 100, 60};
    overall_header = {header_holder_rect.x+670, header_holder_rect.y+header_holder_rect.h/4+2, 100, 60};

    //menu
    menu_bg = IMG_LoadTexture(gRenderer, "menu/background.png");
    menu_bg_rect = {0, 0, width, height};
    menu_header = IMG_LoadTexture(gRenderer, "menu/header.png");
    menu_header_rect = {0, 0, width, 100};
    player_header = IMG_LoadTexture(gRenderer, "menu/player_header.png");
    player_header_rect = {width/2-916/2, 100, 916, 43};
    menu_return = IMG_LoadTexture(gRenderer, menu_return_path.c_str());
    menu_return_rect = {30, 30, 100, 50};
    shop = IMG_LoadTexture(gRenderer, shop_path.c_str());
    shop_rect = {1070, 30, 100, 50};

    normal_mode = IMG_LoadTexture(gRenderer, "menu/normal_mode.png");
    infinite_mode = IMG_LoadTexture(gRenderer, "menu/infinite_mode.png");
    timer_mode = IMG_LoadTexture(gRenderer, "menu/timer_mode.png");
    hardcore_mode = IMG_LoadTexture(gRenderer, "menu/hardcore_mode.png");
    normal_mode_rect = {width/2-720/2, height/2+50-422/2, 720, 422};
    infinite_mode_rect = {770+width/2-720/2, height/2+50-422/2, 720, 422};
    timer_mode_rect = {770*2+width/2-720/2, height/2+50-422/2, 720, 422};
    hardcore_mode_rect = {770*3+width/2-720/2, height/2+50-422/2, 720, 422};

    point = IMG_LoadTexture(gRenderer, "menu/point.png.");
    pointBig = IMG_LoadTexture(gRenderer, "menu/pointBig.png.");
    button1 = IMG_LoadTexture(gRenderer, "menu/button1.png");
    button2 = IMG_LoadTexture(gRenderer, "menu/button2.png");
    button3 = IMG_LoadTexture(gRenderer, "menu/button3.png");
    lock1 = IMG_LoadTexture(gRenderer, "menu/lock1.png");
    lock2 = IMG_LoadTexture(gRenderer, "menu/lock2.png");
    medal1 = IMG_LoadTexture(gRenderer, "menu/medal1.png");
    medal2 = IMG_LoadTexture(gRenderer, "menu/medal2.png");
    medal3 = IMG_LoadTexture(gRenderer, "menu/medal3.png");

    button1Big = IMG_LoadTexture(gRenderer, "menu/button1Big.png");
    button2Big = IMG_LoadTexture(gRenderer, "menu/button2Big.png");
    button3Big = IMG_LoadTexture(gRenderer, "menu/button3Big.png");
    lock1Big = IMG_LoadTexture(gRenderer, "menu/lock1Big.png");
    lock2Big = IMG_LoadTexture(gRenderer, "menu/lock2Big.png");
    medal1Big = IMG_LoadTexture(gRenderer, "menu/medal1Big.png");
    medal2Big = IMG_LoadTexture(gRenderer, "menu/medal2Big.png");
    medal3Big = IMG_LoadTexture(gRenderer, "menu/medal3Big.png");

    norm_button1_rect = {normal_mode_rect.x+90, normal_mode_rect.y+257, 119, 147};
    norm_button2_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 147};
    norm_button3_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 147};
    norm_lock1_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 147};
    norm_lock2_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 147};
    norm_medal1_rect = {normal_mode_rect.x+90, normal_mode_rect.y+257, 119, 159};
    norm_medal2_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 159};
    norm_medal3_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 158};

    inf_button1_rect = {infinite_mode_rect.x+90, infinite_mode_rect.y+257, 119, 147};
    inf_button2_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 147};
    inf_button3_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 147};
    inf_lock1_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 147};
    inf_lock2_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 147};
    inf_medal1_rect = {infinite_mode_rect.x+90, infinite_mode_rect.y+257, 119, 159};
    inf_medal2_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 159};
    inf_medal3_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 158};

    time_button1_rect = {timer_mode_rect.x+90, timer_mode_rect.y+257, 119, 147};
    time_button2_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 147};
    time_button3_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 147};
    time_lock1_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 147};
    time_lock2_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 147};
    time_medal1_rect = {timer_mode_rect.x+90, timer_mode_rect.y+257, 119, 159};
    time_medal2_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 159};
    time_medal3_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 158};

    hard_button1_rect = {hardcore_mode_rect.x+90, hardcore_mode_rect.y+257, 119, 147};
    hard_button2_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 147};
    hard_button3_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 147};
    hard_lock1_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 147};
    hard_lock2_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 147};
    hard_medal1_rect = {hardcore_mode_rect.x+90, hardcore_mode_rect.y+257, 119, 159};
    hard_medal2_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 159};
    hard_medal3_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 158};


    //register
    exit_btn = IMG_LoadTexture(gRenderer, "home/exit_btn.png");
    register_exit_rect = {register_window.x+register_window.w, register_window.y+register_window.h/2-25, 80, 50};

    field_holder = IMG_LoadTexture(gRenderer, "menu/field_holder.png");
    user_rect = {register_window.x+register_window.w/2-380/2, register_window.y+70, 380, 60};
    pass_rect = {register_window.x+register_window.w/2-380/2, register_window.y+160, 380, 60};

    login_btn = IMG_LoadTexture(gRenderer, login_btn_path.c_str());
    register_btn = IMG_LoadTexture(gRenderer, register_btn_path.c_str());
    login_rect = {register_window.x+register_window.w/2-235/2, pass_rect.y+pass_rect.h+50, 235, 64};
    register_rect = {register_window.x+register_window.w/2-235/2, login_rect.y+login_rect.h+30, 235, 64};

    error_rect = {register_window.x+register_window.w/2-360/2+50, (login_rect.y-pass_rect.y-pass_rect.h)/2+pass_rect.y+pass_rect.h-15, 360, 60};

    //end
    end_bg = IMG_LoadTexture(gRenderer, "end/background.png");
    end_window = IMG_LoadTexture(gRenderer, "end/end_window.png");
    to_home = IMG_LoadTexture(gRenderer, to_home_path.c_str());
    to_menu = IMG_LoadTexture(gRenderer, to_menu_path.c_str());
    replay = IMG_LoadTexture(gRenderer, replay_path.c_str());
    win_sign = IMG_LoadTexture(gRenderer, "end/win_sign.png");
    lose_sign = IMG_LoadTexture(gRenderer,"end/lose_sign.png");
    result_sign_rect = {width/2-686/2, height-264, 686, 263};
    end_bg_rect = {0, 0, width, height};
    end_window_rect = {width/2-686/2, height/2-750/2, 686, 750};
    to_home_rect = {30, 300, 100, 50};
    to_menu_rect = {1070, 300, 100, 50};
    replay_rect = {1070, 380, 100, 50};

    //game textures
    wall_texture = IMG_LoadTexture(gRenderer, "game/wall.png");
    wire = IMG_LoadTexture(gRenderer, "game/wire.png");
    wire_rect = {200, 600-8, 800, 16};
    cannon = IMG_LoadTexture(gRenderer, "game/cannon.png");
    cannon_rect = {width/2-78/2, height-130, 78, 116};
    bullet = IMG_LoadTexture(gRenderer, "game/bullet.png");
    bullet_rect = {width-252/2-58/4, height-50-378, 58, 378};
    magic_rect1 = {bullet_rect.x+6, height-58-48-24+6, 48, 48};
    magic_rect2 = {bullet_rect.x+6, height-58-48*2-24+6, 48, 48};
    magic_rect3 = {bullet_rect.x+6, height-58-48*3-24+6, 48, 48};
    magic_rect4 = {bullet_rect.x+6, height-58-48*4-24+6, 48, 48};

    game_setting_window = IMG_LoadTexture(gRenderer, "game/setting_window.png");
    game_exit_window = IMG_LoadTexture(gRenderer, "game/exit_window.png");
    game_exit_yes = IMG_LoadTexture(gRenderer, "game/exit_yes.png");
    game_exit_btn = IMG_LoadTexture(gRenderer, "game/exit_btn.png");
    game_setting_btn = IMG_LoadTexture(gRenderer, "game/setting_btn.png");
    game_defeat = IMG_LoadTexture(gRenderer, "game/defeat.png");
    game_music = IMG_LoadTexture(gRenderer, game_music_path.c_str());

    game_window_rect = {width/2-500/2, height/2-247/2, 500, 247};
    game_exit_yes_rect = {game_window_rect.x - 80, game_window_rect.y+game_window_rect.h/2-25, 80, 50};
    game_defeat_rect = {game_window_rect.x - 80, game_window_rect.y+23, 80, 50};
    game_setting_btn_rect = {game_window_rect.x - 80, game_window_rect.y+23*2+50, 80, 50};
    game_exit_rect = {game_window_rect.x - 80, game_window_rect.y+23*3+50*2, 80, 50};
    game_resume_rect = {game_window_rect.x+game_window_rect.w, game_window_rect.y+game_window_rect.h/2-50/2, 80, 50};
    game_music_rect = {width/2-280/2, game_window_rect.y+146, 280, 65};

    gchunk_toggle_rect = {width/2-40/2, game_window_rect.y+68-40/2, 40, 40};
    gmusic_toggle_rect = {width/2-40/2, game_window_rect.y+112-40/2, 40, 40};
    game_chunk_fissure = {width/2-300/2, game_window_rect.y+58-40/2, 480, 60};
    game_music_fissure = {width/2-300/2, game_window_rect.y+102-40/2, 480, 60};

    frame1 = IMG_LoadTexture(gRenderer, "game/frame1.png");
    frame2 = IMG_LoadTexture(gRenderer, "game/frame2.png");
    frame3 = IMG_LoadTexture(gRenderer, "game/frame3.png");
    frame4 = IMG_LoadTexture(gRenderer, "game/frame4.png");
    frame5 = IMG_LoadTexture(gRenderer, "game/frame5.png");
    frame6 = IMG_LoadTexture(gRenderer, "game/frame6.png");
    frame7 = IMG_LoadTexture(gRenderer, "game/frame7.png");
    frame8 = IMG_LoadTexture(gRenderer, "game/frame8.png");
    frame9 = IMG_LoadTexture(gRenderer, "game/frame9.png");

    fram1 = IMG_LoadTexture(gRenderer, "game/fram0.png");
    fram2 = IMG_LoadTexture(gRenderer, "game/fram1.png");
    fram3 = IMG_LoadTexture(gRenderer, "game/fram2.png");
    fram4 = IMG_LoadTexture(gRenderer, "game/fram3.png");

    laser_rect = {width/2-40/2, -94, 40, 830};

    fr1 = IMG_LoadTexture(gRenderer, "game/fr1.png");
    fr2 = IMG_LoadTexture(gRenderer, "game/fr2.png");
    fr3 = IMG_LoadTexture(gRenderer, "game/fr3.png");
    fr4 = IMG_LoadTexture(gRenderer, "game/fr4.png");
    fr5 = IMG_LoadTexture(gRenderer, "game/fr5.png");
    fr6 = IMG_LoadTexture(gRenderer, "game/fr6.png");
    fr7 = IMG_LoadTexture(gRenderer, "game/fr7.png");
    fr8 = IMG_LoadTexture(gRenderer, "game/fr8.png");
    fr9 = IMG_LoadTexture(gRenderer, "game/fr9.png");

    //ball texture init
    chain = IMG_LoadTexture(gRenderer, "game/chain.png");

    texture_r = IMG_LoadTexture(gRenderer, "balls/red.png");
    texture_g = IMG_LoadTexture(gRenderer, "balls/green.png");
    texture_b = IMG_LoadTexture(gRenderer, "balls/blue.png");
    texture_p = IMG_LoadTexture(gRenderer, "balls/purple.png");
    texture_y = IMG_LoadTexture(gRenderer, "balls/yellow.png");
    texture_o = IMG_LoadTexture(gRenderer, "balls/orange.png");

    texture_bg = IMG_LoadTexture(gRenderer, "balls/blue-green.png");
    texture_br = IMG_LoadTexture(gRenderer, "balls/blue-red.png");
    texture_by = IMG_LoadTexture(gRenderer, "balls/blue-yellow.png");
    texture_bp = IMG_LoadTexture(gRenderer, "balls/blue-purple.png");
    texture_bo = IMG_LoadTexture(gRenderer, "balls/blue-orange.png");
    texture_gr = IMG_LoadTexture(gRenderer, "balls/green-red.png");
    texture_gy = IMG_LoadTexture(gRenderer, "balls/green-yellow.png");
    texture_gp = IMG_LoadTexture(gRenderer, "balls/green-purple.png");
    texture_go = IMG_LoadTexture(gRenderer, "balls/green-orange.png");
    texture_ry = IMG_LoadTexture(gRenderer, "balls/red-yellow.png");
    texture_rp = IMG_LoadTexture(gRenderer, "balls/red-purple.png");
    texture_ro = IMG_LoadTexture(gRenderer, "balls/red-orange.png");
    texture_yp = IMG_LoadTexture(gRenderer, "balls/yellow-purple.png");
    texture_yo = IMG_LoadTexture(gRenderer, "balls/yellow-orange.png");
    texture_po = IMG_LoadTexture(gRenderer, "balls/purple-orange.png");

    texture_all = IMG_LoadTexture(gRenderer, "balls/all.png");
    texture_stone = IMG_LoadTexture(gRenderer, "balls/stone.png");
    texture_bomb = IMG_LoadTexture(gRenderer, "balls/bomb.png");
    texture_laser = IMG_LoadTexture(gRenderer, "balls/laser.png");
    texture_thunder = IMG_LoadTexture(gRenderer, "balls/thunder.png");
    texture_slow = IMG_LoadTexture(gRenderer, "game/slow.png");
    texture_stop = IMG_LoadTexture(gRenderer, "game/stop.png");
    BallRect = {0,0,48, 48};

    //game loop
    while (!quit) {
        while(gHome){
            home(gRenderer);
        }

        while(gMenu){
            menu(gRenderer);
        }

        while(gNormal){
            normal(gRenderer);
        }

        while(gInfinite){
            infinite(gRenderer);
        }

        while(gTimer){
            timer(gRenderer);
        }

        while(gHardcore){
            //hardcore(gRenderer);
        }
        if(gEnd){
            Mix_HaltMusic();
        }
        while(gEnd){
            end(gRenderer, mode);
        }
    }
    if(!open_register){
        pull_db();
        for(int i=0; i<record_vec.size(); i++){
            if(record_vec[i].username == user){
                record_vec[i].norm=norm_score;
                record_vec[i].inf=inf_score;
                record_vec[i].timer=time_score;
                record_vec[i].hard=hard_score;
                record_vec[i].magic1 = colorfulC;
                record_vec[i].magic2 = bombC;
                record_vec[i].magic3 = laserC;
                record_vec[i].magic4 = thunderC;
                record_vec[i].sound=soundP;
                record_vec[i].chunk=chunkP;
                record_vec[i].theme=themeP;
                record_vec[i].back = backgroundP;
                record_vec[i].ball = ballP;
                record_vec[i].cannon = cannonP;
                record_vec[i].music = musicP;
                break;
            }
        }

        fstream spear("db.txt", ios::out);
        spear << record_vec[0].username << " "
              << record_vec[0].password << " "
              << record_vec[0].norm << " "
              << record_vec[0].inf << " "
              << record_vec[0].timer << " "
              << record_vec[0].hard << " "
              << record_vec[0].magic1<< " "
                << record_vec[0].magic2<< " "
                << record_vec[0].magic3<< " "
                << record_vec[0].magic4<< " "
                << record_vec[0].sound << " "
                << record_vec[0].chunk << " "
                << record_vec[0].theme << " "
                << record_vec[0].back<< " "
                << record_vec[0].ball<< " "
                << record_vec[0].cannon<< " "
                << record_vec[0].music<<endl;
        spear.close();

        fstream hello("db.txt", ios::app);
        for (int i = 1; i < record_vec.size(); i++) {
            hello << record_vec[i].username << " "
                  << record_vec[i].password << " "
                  << record_vec[i].norm << " "
                  << record_vec[i].inf << " "
                  << record_vec[i].timer << " "
                  << record_vec[i].hard << " "
                  << record_vec[i].magic1 << " "
                  << record_vec[i].magic2 << " "
                  << record_vec[i].magic3 << " "
                  << record_vec[i].magic4 << " "
                  << record_vec[i].sound << " "
                  << record_vec[i].chunk << " "
                  << record_vec[i].theme << " "
                  << record_vec[i].back << " "
                  << record_vec[i].ball << " "
                  << record_vec[i].cannon << " "
                  << record_vec[i].music <<endl;
        }
        hello.close();
    }

    SDL_DestroyTexture(home_bg);
    SDL_DestroyTexture(wall_texture);
    SDL_DestroyTexture(wire);
    SDL_DestroyTexture(cannon);
    SDL_DestroyTexture(bullet);
    SDL_DestroyTexture(frame1);
    SDL_DestroyTexture(frame2);
    SDL_DestroyTexture(frame3);
    SDL_DestroyTexture(frame4);
    SDL_DestroyTexture(frame5);
    SDL_DestroyTexture(frame6);
    SDL_DestroyTexture(frame7);
    SDL_DestroyTexture(frame8);
    SDL_DestroyTexture(frame9);
    SDL_DestroyTexture(fr1);
    SDL_DestroyTexture(fr2);
    SDL_DestroyTexture(fr3);
    SDL_DestroyTexture(fr4);
    SDL_DestroyTexture(fr5);
    SDL_DestroyTexture(fr6);
    SDL_DestroyTexture(fr7);
    SDL_DestroyTexture(fr8);
    SDL_DestroyTexture(fr9);
    SDL_DestroyTexture(game_setting_window);
    SDL_DestroyTexture(game_exit_window);
    SDL_DestroyTexture(game_exit_yes);
    SDL_DestroyTexture(game_defeat);
    SDL_DestroyTexture(game_exit_btn);
    SDL_DestroyTexture(game_setting_btn);
    SDL_DestroyTexture(game_music);

    SDL_DestroyTexture(menu_header);

    SDL_DestroyTexture(facebook);
    SDL_DestroyTexture(youtube);
    SDL_DestroyTexture(instagram);
    SDL_DestroyTexture(email);

    SDL_DestroyTexture(rate);
    SDL_DestroyTexture(home_setting);
    SDL_DestroyTexture(credit);

    SDL_DestroyTexture(chunk_toggle);
    SDL_DestroyTexture(theme);
    SDL_DestroyTexture(music);
    SDL_DestroyTexture(setting_window);
    SDL_DestroyTexture(theme_btn);
    SDL_DestroyTexture(ball_btn);
    SDL_DestroyTexture(cannon_btn);
    SDL_DestroyTexture(background_btn);

    SDL_DestroyTexture(safari);
    SDL_DestroyTexture(frozen);
    SDL_DestroyTexture(jungle);
    SDL_DestroyTexture(pacific);
    SDL_DestroyTexture(settings_return);
    SDL_DestroyTexture(wood_canon);
    SDL_DestroyTexture(metal_canon);
    SDL_DestroyTexture(beach_ball);
    SDL_DestroyTexture(reg_ball);
    SDL_DestroyTexture(wood_theme);
    SDL_DestroyTexture(metal_theme);
    SDL_DestroyTexture(war);
    SDL_DestroyTexture(peace);

    SDL_DestroyTexture(start);
    SDL_DestroyTexture(ranking);

    SDL_DestroyTexture(login_btn);
    SDL_DestroyTexture(register_btn);
    SDL_DestroyTexture(registration);
    SDL_DestroyTexture(field_holder);
    SDL_DestroyTexture(exit_btn);

    SDL_DestroyTexture(ranking_window);
    SDL_DestroyTexture(info_holder);
    SDL_DestroyTexture(header_holder);
    SDL_DestroyTexture(user_icon);

    SDL_DestroyTexture(gold);
    SDL_DestroyTexture(silver);
    SDL_DestroyTexture(bronze);

    SDL_DestroyTexture(menu_bg);
    SDL_DestroyTexture(menu_header);
    SDL_DestroyTexture(player_header);
    SDL_DestroyTexture(menu_return);
    SDL_DestroyTexture(shop);

    SDL_DestroyTexture(point);
    SDL_DestroyTexture(pointBig);

    SDL_DestroyTexture(normal_mode);
    SDL_DestroyTexture(infinite_mode);
    SDL_DestroyTexture(timer_mode);
    SDL_DestroyTexture(hardcore_mode);

    SDL_DestroyTexture(button1);
    SDL_DestroyTexture(button2);
    SDL_DestroyTexture(button3);
    SDL_DestroyTexture(button1Big);
    SDL_DestroyTexture(button2Big);
    SDL_DestroyTexture(button3Big);
    SDL_DestroyTexture(lock1);
    SDL_DestroyTexture(lock2);
    SDL_DestroyTexture(lock1Big);
    SDL_DestroyTexture(lock2Big);
    SDL_DestroyTexture(medal1);
    SDL_DestroyTexture(medal2);
    SDL_DestroyTexture(medal3);
    SDL_DestroyTexture(medal1Big);
    SDL_DestroyTexture(medal2Big);
    SDL_DestroyTexture(medal3Big);

    SDL_DestroyTexture(end_bg);
    SDL_DestroyTexture(end_window);
    SDL_DestroyTexture(to_home);
    SDL_DestroyTexture(to_menu);
    SDL_DestroyTexture(replay);

    SDL_DestroyTexture(win_sign);
    SDL_DestroyTexture(lose_sign);

    SDL_DestroyTexture(texture_r);
    SDL_DestroyTexture(texture_g);
    SDL_DestroyTexture(texture_b);
    SDL_DestroyTexture(texture_p);
    SDL_DestroyTexture(texture_y);
    SDL_DestroyTexture(texture_o);
    SDL_DestroyTexture(texture_bg);
    SDL_DestroyTexture(texture_br);
    SDL_DestroyTexture(texture_by);
    SDL_DestroyTexture(texture_bp);
    SDL_DestroyTexture(texture_bo);
    SDL_DestroyTexture(texture_gr);
    SDL_DestroyTexture(texture_gy);
    SDL_DestroyTexture(texture_gp);
    SDL_DestroyTexture(texture_go);
    SDL_DestroyTexture(texture_ry);
    SDL_DestroyTexture(texture_rp);
    SDL_DestroyTexture(texture_ro);
    SDL_DestroyTexture(texture_yp);
    SDL_DestroyTexture(texture_yo);
    SDL_DestroyTexture(texture_po);

    SDL_DestroyTexture(chain);
    SDL_DestroyTexture(texture_slow);
    SDL_DestroyTexture(texture_stop);
    SDL_DestroyTexture(texture_all);
    SDL_DestroyTexture(texture_laser);
    SDL_DestroyTexture(texture_stone);
    SDL_DestroyTexture(texture_bomb);
    SDL_DestroyTexture(texture_thunder);

    SDL_FreeSurface(textSur);
    SDL_DestroyTexture(textTex);

    IMG_Quit();

    TTF_CloseFont(font_rank);
    TTF_CloseFont(font_py_hd);
    TTF_CloseFont(font_register);
    TTF_Quit();

    Mix_FreeMusic(frozen_music);
    Mix_FreeMusic(safari_music);
    Mix_FreeMusic(jungle_music);
    Mix_FreeMusic(pacific_music);
    Mix_FreeMusic(game_win_music);
    Mix_FreeMusic(game_lose_music);
    Mix_FreeMusic(background_music);
    Mix_FreeChunk(toggle_chunk);
    Mix_FreeChunk(bomb_explosion_chunk);
    Mix_FreeChunk(ball_explosion_chunk);
    Mix_FreeChunk(laser_chunk);
    Mix_FreeChunk(thunder_chunk);
    Mix_Quit();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    return 0;
}

bool button(SDL_Rect Rect){
    if(m_x>Rect.x && m_x<Rect.x+Rect.w && m_y>Rect.y && m_y<Rect.y+Rect.h){
        return true;
    }
    else{
        return false;
    }
}

void change_theme(bool metal){
    if(metal){
        chunk_toggle = IMG_LoadTexture(gRenderer, "home/chunk_toggle.png");
        music_toggle = IMG_LoadTexture(gRenderer, "home/music_toggle.png");
        wood_theme = IMG_LoadTexture(gRenderer, "home/wood_theme.png");
        metal_theme = IMG_LoadTexture(gRenderer, "home/metal_theme.png");
        war = IMG_LoadTexture(gRenderer, "home/war.png");
        peace = IMG_LoadTexture(gRenderer, "home/peace.png");
        beach_ball = IMG_LoadTexture(gRenderer, "home/beach_ball.png");
        reg_ball = IMG_LoadTexture(gRenderer, "home/reg_ball.png");
        wood_canon = IMG_LoadTexture(gRenderer, "home/wood_canon.png");
        metal_canon = IMG_LoadTexture(gRenderer, "home/metal_canon.png");

        game_defeat = IMG_LoadTexture(gRenderer, "game/defeat.png");
        game_setting_btn = IMG_LoadTexture(gRenderer, "game/setting_btn.png");
        game_exit_btn = IMG_LoadTexture(gRenderer, "game/exit_btn.png");
        game_exit_yes = IMG_LoadTexture(gRenderer, "game/exit_yes.png");
        game_exit_window = IMG_LoadTexture(gRenderer, "game/exit_window.png");
        game_setting_window = IMG_LoadTexture(gRenderer, "game/setting_window.png");
        wall_texture = IMG_LoadTexture(gRenderer, "game/wall.png");
        exit_btn = IMG_LoadTexture(gRenderer, "home/exit_btn.png");
        button1 = IMG_LoadTexture(gRenderer, "menu/button1.png");
        button2 = IMG_LoadTexture(gRenderer, "menu/button2.png");
        button3 = IMG_LoadTexture(gRenderer, "menu/button3.png");
        lock1 = IMG_LoadTexture(gRenderer, "menu/lock1.png");
        lock2 = IMG_LoadTexture(gRenderer, "menu/lock2.png");
        medal1 = IMG_LoadTexture(gRenderer, "menu/medal1.png");
        medal2 = IMG_LoadTexture(gRenderer, "menu/medal2.png");
        medal3 = IMG_LoadTexture(gRenderer, "menu/medal3.png");
        button1Big = IMG_LoadTexture(gRenderer, "menu/button1Big.png");
        button2Big = IMG_LoadTexture(gRenderer, "menu/button2Big.png");
        button3Big = IMG_LoadTexture(gRenderer, "menu/button3Big.png");
        lock1Big = IMG_LoadTexture(gRenderer, "menu/lock1Big.png");
        lock2Big = IMG_LoadTexture(gRenderer, "menu/lock2Big.png");
        medal1Big = IMG_LoadTexture(gRenderer, "menu/medal1Big.png");
        medal2Big = IMG_LoadTexture(gRenderer, "menu/medal2Big.png");
        medal3Big = IMG_LoadTexture(gRenderer, "menu/medal3Big.png");
        normal_mode = IMG_LoadTexture(gRenderer, "menu/normal_mode.png");
        infinite_mode = IMG_LoadTexture(gRenderer, "menu/infinite_mode.png");
        timer_mode = IMG_LoadTexture(gRenderer, "menu/timer_mode.png");
        hardcore_mode = IMG_LoadTexture(gRenderer, "menu/hardcore_mode.png");
        menu_header = IMG_LoadTexture(gRenderer, "menu/header.png");
        player_header = IMG_LoadTexture(gRenderer, "menu/player_header.png");

        rate_path="home/rate.png";
        rate= IMG_LoadTexture(gRenderer, rate_path.c_str());
        home_setting_path="home/home_setting.png";
        home_setting = IMG_LoadTexture(gRenderer, home_setting_path.c_str());
        credit_path="home/credit.png";
        credit = IMG_LoadTexture(gRenderer, credit_path.c_str());
        start_path="home/start.png";
        start = IMG_LoadTexture(gRenderer, start_path.c_str());
        ranking_path="home/ranking.png";
        ranking = IMG_LoadTexture(gRenderer, ranking_path.c_str());
        login_btn_path="menu/login.png";
        login_btn = IMG_LoadTexture(gRenderer, login_btn_path.c_str());
        register_btn_path="menu/register.png";
        register_btn = IMG_LoadTexture(gRenderer, register_btn_path.c_str());
        theme_path="home/theme.png";
        theme = IMG_LoadTexture(gRenderer, theme_path.c_str());
        music_path="home/music.png";
        music = IMG_LoadTexture(gRenderer,music_path.c_str());
        theme_btn_path="home/theme_btn.png";
        theme_btn = IMG_LoadTexture(gRenderer, theme_btn_path.c_str());
        ball_btn_path="home/ball_btn.png";
        ball_btn= IMG_LoadTexture(gRenderer, ball_btn_path.c_str());
        cannon_btn_path="home/cannon_btn.png";
        cannon_btn = IMG_LoadTexture(gRenderer, cannon_btn_path.c_str());
        background_btn_path="home/background_btn.png";
        background_btn = IMG_LoadTexture(gRenderer, background_btn_path.c_str());
        safari_path="home/safari.png";
        safari = IMG_LoadTexture(gRenderer, safari_path.c_str());
        jungle_path="home/jungle.png";
        jungle = IMG_LoadTexture(gRenderer, jungle_path.c_str());
        frozen_path="home/frozen.png";
        frozen = IMG_LoadTexture(gRenderer, frozen_path.c_str());
        pacific_path="home/pacific.png";
        pacific = IMG_LoadTexture(gRenderer, pacific_path.c_str());
        menu_return_path="menu/return.png";
        menu_return = IMG_LoadTexture(gRenderer, menu_return_path.c_str());
        settings_return = IMG_LoadTexture(gRenderer, "home/return.png");
        shop_path="menu/shop.png";
        shop = IMG_LoadTexture(gRenderer, shop_path.c_str());
        to_menu_path="end/to_menu.png";
        to_menu = IMG_LoadTexture(gRenderer, to_menu_path.c_str());
        to_home_path="end/to_home.png";
        to_home = IMG_LoadTexture(gRenderer, to_home_path.c_str());
        replay_path="end/replay.png";
        replay = IMG_LoadTexture(gRenderer, replay_path.c_str());
    }
    else{
        chunk_toggle = IMG_LoadTexture(gRenderer, "home1/chunk_toggle.png");
        music_toggle = IMG_LoadTexture(gRenderer, "home1/music_toggle.png");
        chunk_toggle = IMG_LoadTexture(gRenderer, "home1/chunk_toggle.png");
        music_toggle = IMG_LoadTexture(gRenderer, "home1/music_toggle.png");
        wood_theme = IMG_LoadTexture(gRenderer, "home1/wood_theme.png");
        metal_theme = IMG_LoadTexture(gRenderer, "home1/metal_theme.png");
        war = IMG_LoadTexture(gRenderer, "home1/war.png");
        peace = IMG_LoadTexture(gRenderer, "home1/peace.png");
        beach_ball = IMG_LoadTexture(gRenderer, "home1/beach_ball.png");
        reg_ball = IMG_LoadTexture(gRenderer, "home1/reg_ball.png");
        wood_canon = IMG_LoadTexture(gRenderer, "home1/wood_canon.png");
        metal_canon = IMG_LoadTexture(gRenderer, "home1/metal_canon.png");

        game_defeat = IMG_LoadTexture(gRenderer, "game1/defeat.png");
        game_setting_btn = IMG_LoadTexture(gRenderer, "game1/setting_btn.png");
        game_exit_btn = IMG_LoadTexture(gRenderer, "game1/exit_btn.png");
        game_exit_yes = IMG_LoadTexture(gRenderer, "game1/exit_yes.png");
        game_exit_window = IMG_LoadTexture(gRenderer, "game1/exit_window.png");
        game_setting_window = IMG_LoadTexture(gRenderer, "game1/setting_window.png");
        wall_texture = IMG_LoadTexture(gRenderer, "game1/wall.png");
        exit_btn = IMG_LoadTexture(gRenderer, "home1/exit_btn.png");
        button1 = IMG_LoadTexture(gRenderer, "menu1/button1.png");
        button2 = IMG_LoadTexture(gRenderer, "menu1/button2.png");
        button3 = IMG_LoadTexture(gRenderer, "menu1/button3.png");
        lock1 = IMG_LoadTexture(gRenderer, "menu1/lock1.png");
        lock2 = IMG_LoadTexture(gRenderer, "menu1/lock2.png");
        medal1 = IMG_LoadTexture(gRenderer, "menu1/medal1.png");
        medal2 = IMG_LoadTexture(gRenderer, "menu1/medal2.png");
        medal3 = IMG_LoadTexture(gRenderer, "menu1/medal3.png");
        button1Big = IMG_LoadTexture(gRenderer, "menu1/button1Big.png");
        button2Big = IMG_LoadTexture(gRenderer, "menu1/button2Big.png");
        button3Big = IMG_LoadTexture(gRenderer, "menu1/button3Big.png");
        lock1Big = IMG_LoadTexture(gRenderer, "menu1/lock1Big.png");
        lock2Big = IMG_LoadTexture(gRenderer, "menu1/lock2Big.png");
        medal1Big = IMG_LoadTexture(gRenderer, "menu1/medal1Big.png");
        medal2Big = IMG_LoadTexture(gRenderer, "menu1/medal2Big.png");
        medal3Big = IMG_LoadTexture(gRenderer, "menu1/medal3Big.png");
        normal_mode = IMG_LoadTexture(gRenderer, "menu1/normal_mode.png");
        infinite_mode = IMG_LoadTexture(gRenderer, "menu1/infinite_mode.png");
        timer_mode = IMG_LoadTexture(gRenderer, "menu1/timer_mode.png");
        hardcore_mode = IMG_LoadTexture(gRenderer, "menu1/hardcore_mode.png");
        menu_header = IMG_LoadTexture(gRenderer, "menu1/header.png");
        player_header = IMG_LoadTexture(gRenderer, "menu1/player_header.png");

        rate_path="home1/rate.png";
        rate= IMG_LoadTexture(gRenderer, rate_path.c_str());
        home_setting_path="home1/home_setting.png";
        home_setting = IMG_LoadTexture(gRenderer, home_setting_path.c_str());
        credit_path="home1/credit.png";
        credit = IMG_LoadTexture(gRenderer, credit_path.c_str());
        start_path="home1/start.png";
        start = IMG_LoadTexture(gRenderer, start_path.c_str());
        ranking_path="home1/ranking.png";
        ranking = IMG_LoadTexture(gRenderer, ranking_path.c_str());
        login_btn_path="menu1/login.png";
        login_btn = IMG_LoadTexture(gRenderer, login_btn_path.c_str());
        register_btn_path="menu1/register.png";
        register_btn = IMG_LoadTexture(gRenderer, register_btn_path.c_str());
        theme_path="home1/theme.png";
        theme = IMG_LoadTexture(gRenderer, theme_path.c_str());
        music_path="home1/music.png";
        music = IMG_LoadTexture(gRenderer,music_path.c_str());
        theme_btn_path="home1/theme_btn.png";
        theme_btn = IMG_LoadTexture(gRenderer, theme_btn_path.c_str());
        ball_btn_path="home1/ball_btn.png";
        ball_btn= IMG_LoadTexture(gRenderer, ball_btn_path.c_str());
        cannon_btn_path="home1/cannon_btn.png";
        cannon_btn = IMG_LoadTexture(gRenderer, cannon_btn_path.c_str());
        background_btn_path="home1/background_btn.png";
        background_btn = IMG_LoadTexture(gRenderer, background_btn_path.c_str());
        safari_path="home1/safari.png";
        safari = IMG_LoadTexture(gRenderer, safari_path.c_str());
        jungle_path="home1/jungle.png";
        jungle = IMG_LoadTexture(gRenderer, jungle_path.c_str());
        frozen_path="home1/frozen.png";
        frozen = IMG_LoadTexture(gRenderer, frozen_path.c_str());
        pacific_path="home1/pacific.png";
        pacific = IMG_LoadTexture(gRenderer, pacific_path.c_str());
        menu_return_path="menu1/return.png";
        menu_return = IMG_LoadTexture(gRenderer, menu_return_path.c_str());
        settings_return = IMG_LoadTexture(gRenderer, "home1/return.png");
        shop_path="menu1/shop.png";
        shop = IMG_LoadTexture(gRenderer, shop_path.c_str());
        to_menu_path="end1/to_menu.png";
        to_menu = IMG_LoadTexture(gRenderer, to_menu_path.c_str());
        to_home_path="end1/to_home.png";
        to_home = IMG_LoadTexture(gRenderer, to_home_path.c_str());
        replay_path="end1/replay.png";
        replay = IMG_LoadTexture(gRenderer, replay_path.c_str());
    }
}

void change_ball_color(bool beach){
    if(beach){
        texture_r = IMG_LoadTexture(gRenderer, "balls1/red.png");
        texture_g = IMG_LoadTexture(gRenderer, "balls1/green.png");
        texture_b = IMG_LoadTexture(gRenderer, "balls1/blue.png");
        texture_p = IMG_LoadTexture(gRenderer, "balls1/purple.png");
        texture_y = IMG_LoadTexture(gRenderer, "balls1/yellow.png");
        texture_o = IMG_LoadTexture(gRenderer, "balls1/orange.png");

        texture_bg = IMG_LoadTexture(gRenderer, "balls1/blue-green.png");
        texture_br = IMG_LoadTexture(gRenderer, "balls1/blue-red.png");
        texture_by = IMG_LoadTexture(gRenderer, "balls1/blue-yellow.png");
        texture_bp = IMG_LoadTexture(gRenderer, "balls1/blue-purple.png");
        texture_bo = IMG_LoadTexture(gRenderer, "balls1/blue-orange.png");
        texture_gr = IMG_LoadTexture(gRenderer, "balls1/green-red.png");
        texture_gy = IMG_LoadTexture(gRenderer, "balls1/green-yellow.png");
        texture_gp = IMG_LoadTexture(gRenderer, "balls1/green-purple.png");
        texture_go = IMG_LoadTexture(gRenderer, "balls1/green-orange.png");
        texture_ry = IMG_LoadTexture(gRenderer, "balls1/red-yellow.png");
        texture_rp = IMG_LoadTexture(gRenderer, "balls1/red-purple.png");
        texture_ro = IMG_LoadTexture(gRenderer, "balls1/red-orange.png");
        texture_yp = IMG_LoadTexture(gRenderer, "balls1/yellow-purple.png");
        texture_yo = IMG_LoadTexture(gRenderer, "balls1/yellow-orange.png");
        texture_po = IMG_LoadTexture(gRenderer, "balls1/purple-orange.png");

        texture_all = IMG_LoadTexture(gRenderer, "balls1/all.png");
        texture_stone = IMG_LoadTexture(gRenderer, "balls1/stone.png");
    }
    else{
        texture_r = IMG_LoadTexture(gRenderer, "balls/red.png");
        texture_g = IMG_LoadTexture(gRenderer, "balls/green.png");
        texture_b = IMG_LoadTexture(gRenderer, "balls/blue.png");
        texture_p = IMG_LoadTexture(gRenderer, "balls/purple.png");
        texture_y = IMG_LoadTexture(gRenderer, "balls/yellow.png");
        texture_o = IMG_LoadTexture(gRenderer, "balls/orange.png");

        texture_bg = IMG_LoadTexture(gRenderer, "balls/blue-green.png");
        texture_br = IMG_LoadTexture(gRenderer, "balls/blue-red.png");
        texture_by = IMG_LoadTexture(gRenderer, "balls/blue-yellow.png");
        texture_bp = IMG_LoadTexture(gRenderer, "balls/blue-purple.png");
        texture_bo = IMG_LoadTexture(gRenderer, "balls/blue-orange.png");
        texture_gr = IMG_LoadTexture(gRenderer, "balls/green-red.png");
        texture_gy = IMG_LoadTexture(gRenderer, "balls/green-yellow.png");
        texture_gp = IMG_LoadTexture(gRenderer, "balls/green-purple.png");
        texture_go = IMG_LoadTexture(gRenderer, "balls/green-orange.png");
        texture_ry = IMG_LoadTexture(gRenderer, "balls/red-yellow.png");
        texture_rp = IMG_LoadTexture(gRenderer, "balls/red-purple.png");
        texture_ro = IMG_LoadTexture(gRenderer, "balls/red-orange.png");
        texture_yp = IMG_LoadTexture(gRenderer, "balls/yellow-purple.png");
        texture_yo = IMG_LoadTexture(gRenderer, "balls/yellow-orange.png");
        texture_po = IMG_LoadTexture(gRenderer, "balls/purple-orange.png");

        texture_all = IMG_LoadTexture(gRenderer, "balls/all.png");
        texture_stone = IMG_LoadTexture(gRenderer, "balls/stone.png");
    }
}

void text(SDL_Renderer* renderer, int x, int y, const char* text, int r, int g, int b, int a, int n) {
    SDL_Rect rectText{x, y, 0, 0};
    Uint8 rr = r;    Uint8 gg = g;    Uint8 bb = b;    Uint8 aa = a;
    SDL_Color color{rr, gg, bb, aa};

    SDL_Surface* textSur = nullptr;
    SDL_Texture* textTex = nullptr;

    switch (n) {
        case 0:
            textSur = TTF_RenderText_Blended(font_rank, text, color);
            break;
        case 1:
            textSur = TTF_RenderText_Blended(font_py_hd, text, color);
            break;
        case 2:
            textSur = TTF_RenderText_Blended(font_register, text, color);
            break;
    }

    if (textSur != nullptr) {
        textTex = SDL_CreateTextureFromSurface(renderer, textSur);
        SDL_QueryTexture(textTex, nullptr, nullptr, &rectText.w, &rectText.h);
        SDL_RenderCopy(renderer, textTex, nullptr, &rectText);
        SDL_FreeSurface(textSur);
    }

    if (textTex != nullptr) {
        SDL_DestroyTexture(textTex);
    }
}

void home_draw(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, home_bg, NULL, &home_bg_rect);

    SDL_RenderCopy(renderer, facebook, NULL, &facebook_rect);
    SDL_RenderCopy(renderer, youtube, NULL, &youtube_rect);
    SDL_RenderCopy(renderer, instagram, NULL, &instagram_rect);
    SDL_RenderCopy(renderer, email, NULL, &email_rect);

    SDL_RenderCopy(renderer, rate, NULL, &rate_rect);
    SDL_RenderCopy(renderer, home_setting, NULL, &home_setting_rect);
    SDL_RenderCopy(renderer, credit, NULL, &credit_rect);

    SDL_RenderCopy(renderer, start, NULL, &start_rect);
    SDL_RenderCopy(renderer, ranking, NULL, &ranking_rect);
}

void home_focus(){
    if(button(facebook_rect)){
        if(facebook_path=="home/facebook.png"){
            facebook_path="home/facebookBig.png";
            facebook = IMG_LoadTexture(gRenderer, facebook_path.c_str());
            facebook_rect = {40-5, 34-5, 75, 75};
        }
    }
    else{
        if(facebook_path=="home/facebookBig.png"){
            facebook_path="home/facebook.png";
            facebook = IMG_LoadTexture(gRenderer, facebook_path.c_str());
            facebook_rect = {40, 34, 65, 65};
        }
    }

    if(button(youtube_rect)){
        if(youtube_path=="home/youtube.png"){
            youtube_path="home/youtubeBig.png";
            youtube = IMG_LoadTexture(gRenderer, youtube_path.c_str());
            youtube_rect = {40-5, 134-5, 75, 75};
        }
    }
    else{
        if(youtube_path=="home/youtubeBig.png"){
            youtube_path="home/youtube.png";
            youtube = IMG_LoadTexture(gRenderer, youtube_path.c_str());
            youtube_rect = {40, 134, 65, 65};
        }
    }

    if(button(instagram_rect)){
        if(instagram_path=="home/instagram.png"){
            instagram_path="home/instagramBig.png";
            instagram = IMG_LoadTexture(gRenderer, instagram_path.c_str());
            instagram_rect = {40-5, 234-5, 75, 75};
        }
    }
    else{
        if(instagram_path=="home/instagramBig.png"){
            instagram_path="home/instagram.png";
            instagram = IMG_LoadTexture(gRenderer, instagram_path.c_str());
            instagram_rect = {40, 234, 65, 65};
        }
    }

    if(button(email_rect)){
        if(email_path=="home/email.png"){
            email_path="home/emailBig.png";
            email = IMG_LoadTexture(gRenderer, email_path.c_str());
            email_rect = {40-5, 334-5, 75, 75};
        }
    }
    else{
        if(email_path=="home/emailBig.png"){
            email_path="home/email.png";
            email = IMG_LoadTexture(gRenderer, email_path.c_str());
            email_rect = {40, 334, 65, 65};
        }
    }

    if(button(rate_rect)){
        if(rate_path=="home/rate.png" || rate_path=="home1/rate.png"){
            if(themeP==0){
                rate_path="home/rateBig.png";
            }else{
                rate_path="home1/rateBig.png";
            }
            rate = IMG_LoadTexture(gRenderer, rate_path.c_str());
            rate_rect = {1060-7, 34-3, 115,57};
        }
    }
    else{
        if(rate_path=="home/rateBig.png" || rate_path=="home1/rateBig.png"){
            if(themeP==0){
                rate_path="home/rate.png";
            }else{
                rate_path="home1/rate.png";
            }
            rate = IMG_LoadTexture(gRenderer, rate_path.c_str());
            rate_rect = {1060, 34, 100,50};
        }
    }

    if(button(home_setting_rect)){
        if(home_setting_path=="home/home_setting.png" || home_setting_path=="home1/home_setting.png"){
            if(themeP==0){
                home_setting_path="home/home_settingBig.png";
            }else{
                home_setting_path="home1/home_settingBig.png";
            }
            home_setting = IMG_LoadTexture(gRenderer, home_setting_path.c_str());
            home_setting_rect = {1060-7, 134-3, 115,57};

        }
    }
    else{
        if(home_setting_path=="home/home_settingBig.png" || home_setting_path=="home1/home_settingBig.png"){
            if(themeP==0){
                home_setting_path="home/home_setting.png";
            }else{
                home_setting_path="home1/home_setting.png";
            }
            home_setting = IMG_LoadTexture(gRenderer, home_setting_path.c_str());
            home_setting_rect = {1060, 134, 100,50};
        }
    }

    if(button(credit_rect)){
        if(credit_path=="home/credit.png" || credit_path=="home1/credit.png"){
            if(themeP==0){
                credit_path="home/creditBig.png";
            }else{
                credit_path="home1/creditBig.png";
            }
            credit = IMG_LoadTexture(gRenderer, credit_path.c_str());
            credit_rect = {1060-7, 244-3, 115,57};
        }
    }
    else{
        if(credit_path=="home/creditBig.png" || credit_path=="home1/creditBig.png"){
            if(themeP==0){
                credit_path="home/credit.png";
            }else{
                credit_path="home1/credit.png";
            }
            credit = IMG_LoadTexture(gRenderer, credit_path.c_str());
            credit_rect = {1060, 244, 100,50};
        }
    }

    if(button(start_rect)){
        if(start_path=="home/start.png" || start_path=="home1/start.png"){
            if(themeP==0){
                start_path="home/startBig.png";
            }else{
                start_path="home1/startBig.png";
            }
            start = IMG_LoadTexture(gRenderer, start_path.c_str());
            start_rect = {width/2-330/2-25, 480-5, 379, 71};
        }
    }
    else{
        if(start_path=="home/startBig.png" || start_path=="home1/startBig.png"){
            if(themeP==0){
                start_path="home/start.png";
            }else{
                start_path="home1/start.png";
            }
            start = IMG_LoadTexture(gRenderer, start_path.c_str());
            start_rect = {width/2-330/2, 480, 330, 62};
        }
    }

    if(button(ranking_rect)){
        if(ranking_path=="home/ranking.png" || ranking_path=="home1/ranking.png"){
            if(themeP==0){
                ranking_path="home/rankingBig.png";
            }else{
                ranking_path="home1/rankingBig.png";
            }
            ranking = IMG_LoadTexture(gRenderer, ranking_path.c_str());
            ranking_rect = {width/2-330/2-25, 570-5, 379, 71};
        }
    }
    else{
        if(ranking_path=="home/rankingBig.png" || ranking_path=="home1/rankingBig.png"){
            if(themeP==0){
                ranking_path="home/ranking.png";
            }else{
                ranking_path="home1/ranking.png";
            }
            ranking = IMG_LoadTexture(gRenderer, ranking_path.c_str());
            ranking_rect = {width/2-330/2, 570, 330, 62};
        }
    }
}

void draw_register(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, registration, NULL, &register_window);
    SDL_RenderCopy(renderer, exit_btn, NULL, &register_exit_rect);

    SDL_RenderCopy(renderer, field_holder, NULL, &user_rect);
    SDL_RenderCopy(renderer, field_holder, NULL, &pass_rect);

    SDL_RenderCopy(renderer, login_btn, NULL, &login_rect);
    SDL_RenderCopy(renderer, register_btn, NULL, &register_rect);
}

int checkCredentials(const string& username, const string& password){
    ifstream dbFile("db.txt");
    string dbUsername, dbPassword;
    long long dummy;

    while (dbFile >> dbUsername >> dbPassword >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy) {
        if (dbUsername == username && dbPassword == password) {
            return 1;
            // Username and password match found in the database
        }
        if (dbUsername == username && dbPassword != password) {
            return 2;
            // Username match found in the database
        }
        if (dbUsername != username && dbPassword == password) {
            return 3;
            // Password match found in the database
        }
    }
    return 0;
    dbFile.close();
    // No matching username-password pair found in the database
}

void pull_db(){
    if(!record_vec.empty()){
        record_vec.resize(0);
    }

    ifstream file("db.txt");
    string db_user, db_pass;
    long long score1, score2, score3, score4;
    long long m1, m2, m3, m4;
    double sound1, chunk1;
    long long theme1, back1, ball1, cannon1, music1;

    while(file>>db_user>>db_pass>>score1>>score2>>score3>>score4>>m1>>m2>>m3>>m4>>sound1>>chunk1>>theme1>>back1>>ball1>>cannon1>>music1){
        record_vec.push_back({db_user,db_pass, score1, score2, score3, score4, m1, m2, m3, m4, sound1, chunk1, theme1, back1, ball1, cannon1, music1});
    }
    file.close();
}

void compare(int a){
    for(int i=0; i<record_vec.size(); i++){
        for(int j=0; j<record_vec.size()-i-1; j++){
            if(a==0){
                if (record_vec[j].norm < record_vec[j+1].norm){
                    swap(record_vec[j], record_vec[j+1]);
                }
            }
            if(a==1){
                if (record_vec[j].inf < record_vec[j+1].inf){
                    swap(record_vec[j], record_vec[j+1]);
                }
            }
            if(a==2){
                if (record_vec[j].timer < record_vec[j+1].timer){
                    swap(record_vec[j], record_vec[j+1]);
                }
            }
            if(a==3){
                if (record_vec[j].hard < record_vec[j+1].hard){
                    swap(record_vec[j], record_vec[j+1]);
                }
            }
            if(a==4){
                if (record_vec[j].norm+record_vec[j].inf+record_vec[j].timer+record_vec[j].hard < record_vec[j+1].norm+record_vec[j+1].inf+record_vec[j+1].timer+record_vec[j+1].hard){
                    swap(record_vec[j], record_vec[j+1]);
                }
            }
        }
    }
    for(int i=0; i<record_vec.size(); i++){
        record_vec[i].y = header_holder_rect.y+(i+1)*110;
    }
}

void text_writer(SDL_Renderer* renderer){
    text(renderer, user_rect.x+10, user_rect.y+5, user.c_str(), 000, 000, 000, 255, 2);
    text(renderer, pass_rect.x+10, pass_rect.y+5, pass.c_str(), 000, 000, 000, 255, 2);

    if(input && user.length()!=0 && pass.length()!=0){
        int result = checkCredentials(user, pass);

        if(new_player){
            if(result == 0 || result == 3){
                fstream rec("db.txt", ios::app);
                rec<<user<<" "<<pass<<" "<<norm_score<<" "<<inf_score<<" "<<time_score<<" "<<hard_score<<" "<<colorfulC<<" "<<bombC<<" "<<laserC<<" "<<thunderC<<" "<<soundP<<" "<<chunkP<<" "<<themeP<<" "<<backgroundP<<" "<<ballP<<" "<<cannonP<<" "<<musicP<< endl;
                rec.close();
                open_register = false;
                show_register = false;
            }
            else{
                error_reg=true;
                user="";
                pass="";
                last=false;
                input = false;
            }
        }
        else{
            if(result == 1){
                open_register = false;
                show_register = false;
                pull_db();
                for(int i=0; i<record_vec.size(); i++){
                    if(record_vec[i].username == user){
                        norm_score=record_vec[i].norm;
                        inf_score=record_vec[i].inf;
                        time_score=record_vec[i].timer;
                        hard_score=record_vec[i].hard;
                        colorfulC = record_vec[i].magic1;
                        bombC = record_vec[i].magic2;
                        laserC = record_vec[i].magic3;
                        thunderC = record_vec[i].magic4;
                        soundP=record_vec[i].sound;
                        chunkP=record_vec[i].chunk;
                        themeP=record_vec[i].theme;
                        backgroundP = record_vec[i].back;
                        ballP = record_vec[i].ball;
                        cannonP = record_vec[i].cannon;
                        musicP = record_vec[i].music;
                        Mix_HaltMusic();
                        if(musicP==0){
                            Mix_PlayMusic(frozen_music, -1);
                        }
                        if(musicP==1){
                            Mix_PlayMusic(safari_music, -1);
                        }
                        if(musicP==2){
                            Mix_PlayMusic(pacific_music, -1);
                        }
                        if(musicP==3){
                            Mix_PlayMusic(jungle_music, -1);
                        }

                        if(themeP==0){
                            change_theme(true);
                        }
                        else{
                            change_theme(false);
                        }

                        if(cannonP==0){
                            cannon = IMG_LoadTexture(renderer, "game/cannon.png");
                        }
                        else{
                            cannon = IMG_LoadTexture(renderer, "game/cannon_wood.png");
                        }

                        if(ballP==0){
                            change_ball_color(false);
                        }
                        else{
                            change_ball_color(true);
                        }

                        if(backgroundP==0){
                            home_bg = IMG_LoadTexture(renderer, "home/background.png");
                            menu_bg = IMG_LoadTexture(renderer, "menu/background.png");
                            end_bg = IMG_LoadTexture(renderer, "end/background.png");
                        }
                        else{
                            home_bg = IMG_LoadTexture(renderer, "home/background1.png");
                            menu_bg = IMG_LoadTexture(renderer, "menu/background1.png");
                            end_bg = IMG_LoadTexture(renderer, "end/background1.png");
                        }
                        Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*chunkP/360);
                        Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*chunkP/360);
                        Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*chunkP/360);
                        Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*chunkP/360);
                        Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*chunkP/360);
                        Mix_VolumeMusic(MIX_MAX_VOLUME*soundP/360);
                    }
                }
                record_vec.resize(0);
            }
            else{
                error_log=true;
                user="";
                pass="";
                last=false;
                input = false;
            }
        }
    }
}

void draw_home_setting(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, registration, NULL, &register_window);
    SDL_RenderCopy(renderer, exit_btn, NULL, &setting_exit_rect);

    SDL_RenderCopy(renderer, chunk_toggle, NULL, &chunk_toggle_rect);
    SDL_RenderCopy(renderer, music_toggle, NULL, &music_toggle_rect);

    SDL_RenderCopy(renderer, music, NULL, &music_rect);
    SDL_RenderCopy(renderer, theme, NULL, &theme_rect);
}

void rank_scroll(bool u){
    if(!u){
        if(record_vec[record_vec.size()-1].y>ranking_window_rect.y+ranking_window_rect.h){
            for(int i=0; i<record_vec.size(); i++){
                record_vec[i].y-=110;
            }
        }
    }
    else{
        if(record_vec[0].y<=header_holder_rect.y){
            for(int i=0; i<record_vec.size(); i++){
                record_vec[i].y+=110;
            }
        }
    }
}

void draw_ranking(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, ranking_window, NULL, &ranking_window_rect);
    SDL_RenderCopy(renderer, exit_btn, NULL, &ranking_exit_rect);
    SDL_RenderCopy(renderer, header_holder, NULL, &header_holder_rect);
    text(renderer, user_header.x, user_header.y, "Player", 000, 000, 000, 255, 0);
    text(renderer, norm_header.x, norm_header.y, "Normal", 000, 000, 000, 255, 0);
    text(renderer, inf_header.x, inf_header.y, "Infinite", 000, 000, 000, 255, 0);
    text(renderer, time_header.x, time_header.y, "Timer", 000, 000, 000, 255, 0);
    text(renderer, hard_header.x, hard_header.y, "Hardcore", 000, 000, 000, 255, 0);
    text(renderer, overall_header.x, overall_header.y, "Overall", 000, 000, 000, 255, 0);

    for(int i=0; i<record_vec.size(); i++){
        if(record_vec[i].y > header_holder_rect.y+header_holder_rect.h && record_vec[i].y < ranking_window_rect.y+ranking_window_rect.h){
            info_holder_rect.y = record_vec[i].y;
            info.y = info_holder_rect.y;
            user_icon_rect.y = info_holder_rect.y+3;
            medal.y = info_holder_rect.y;
            SDL_RenderCopy(renderer, info_holder, NULL, &info_holder_rect);
            SDL_RenderCopy(renderer, user_icon, NULL, &user_icon_rect);
            switch(i){
                case 0:
                    SDL_RenderCopy(renderer, gold, NULL, &medal);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, silver, NULL, &medal);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, bronze, NULL, &medal);
                    break;
                default:
                    text(renderer, medal.x+medal.w/2-5, medal.y+18, to_string(i).c_str(), 000, 000, 000, 255, 0);
                    break;
            }

            if(!open_register && record_vec[i].username == user){
                if(norm_score > record_vec[i].norm){
                    record_vec[i].norm = norm_score;
                }
                if(inf_score > record_vec[i].inf){
                    record_vec[i].inf = inf_score;
                }
                if(time_score > record_vec[i].timer){
                    record_vec[i].timer = time_score;
                }
                if(hard_score > record_vec[i].hard){
                    record_vec[i].hard = hard_score;
                }
            }

            for(int j=0; j<6; j++){
                info.x = header_holder_rect.x+170+j*100;
                switch(j){
                    case 0:
                        text(renderer, info.x+5, info.y+16, record_vec[i].username.c_str(), 000, 000, 000, 255, 0);
                        break;
                    case 1:
                        text(renderer, info.x+15, info.y+18, to_string(record_vec[i].norm).c_str(), 000, 000, 000, 255, 0);
                        break;
                    case 2:
                        text(renderer, info.x+15, info.y+18, to_string(record_vec[i].inf).c_str(), 000, 000, 000, 255, 0);
                        break;
                    case 3:
                        text(renderer, info.x+15, info.y+18, to_string(record_vec[i].timer).c_str(), 000, 000, 000, 255, 0);
                        break;
                    case 4:
                        text(renderer, info.x+15, info.y+18, to_string(record_vec[i].hard).c_str(), 000, 000, 000, 255, 0);
                        break;
                    case 5:
                        text(renderer, info.x+15, info.y+18, to_string(record_vec[i].norm+record_vec[i].inf+record_vec[i].timer+record_vec[i].hard).c_str(), 000, 000, 000, 255, 0);
                        break;
                }
            }
        }
    }
}

void draw_theme(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, setting_window, NULL, &register_window);
    SDL_RenderCopy(renderer, theme_btn, NULL, &theme_btn_rect);
    SDL_RenderCopy(renderer, background_btn, NULL, &background_btn_rect);
    SDL_RenderCopy(renderer, ball_btn, NULL, &ball_btn_rect);
    SDL_RenderCopy(renderer, cannon_btn, NULL, &cannon_btn_rect);
    SDL_RenderCopy(renderer, settings_return, NULL, &settings_return_rect);
}

void  draw_music(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, setting_window, NULL, &register_window);
    SDL_RenderCopy(renderer, settings_return, NULL, &settings_return_rect);
    SDL_RenderCopy(renderer, safari, NULL, &safari_rect);
    SDL_RenderCopy(renderer, frozen, NULL, &frozen_rect);
    SDL_RenderCopy(renderer, jungle, NULL, &jungle_rect);
    SDL_RenderCopy(renderer, pacific, NULL, &pacific_rect);
    SDL_RenderCopy(renderer, settings_return, NULL, &settings_return_rect);
}

void home_setting_focus(){
    if(button(theme_rect)){
        if(theme_path=="home/theme.png" || theme_path=="home1/theme.png"){
            if(themeP==0){
                theme_path="home/themeBig.png";
            }else{
                theme_path="home1/themeBig.png";
            }
            theme = IMG_LoadTexture(gRenderer, theme_path.c_str());
            theme_rect = {register_window.x+register_window.w/2-340/2-10, register_window.y+244-2, 360, 84};
        }
    }
    else{
        if(theme_path=="home/themeBig.png" || theme_path=="home1/themeBig.png"){
            if(themeP==0){
                theme_path="home/theme.png";
            }else{
                theme_path="home1/theme.png";
            }
            theme = IMG_LoadTexture(gRenderer, theme_path.c_str());
            theme_rect = {register_window.x+register_window.w/2-340/2, register_window.y+244, 340, 80};
        }
    }

    if(button(music_rect)){
        if(music_path=="home/music.png" || music_path=="home1/music.png"){
            if(themeP==0){
                music_path="home/musicBig.png";
            }else{
                music_path="home1/musicBig.png";
            }
            music = IMG_LoadTexture(gRenderer, music_path.c_str());
            music_rect = {register_window.x+register_window.w/2-340/2-10, register_window.y+372-2, 366, 84};
        }
    }
    else{
        if(music_path=="home/musicBig.png" || music_path=="home1/musicBig.png"){
            if(themeP==0){
                music_path="home/music.png";
            }else{
                music_path="home1/music.png";
            }
            music = IMG_LoadTexture(gRenderer, music_path.c_str());
            music_rect = {register_window.x+register_window.w/2-340/2, register_window.y+372, 340, 80};
        }
    }

    if(button(theme_btn_rect)){
        if(theme_btn_path=="home/theme_btn.png" || theme_btn_path=="home1/theme_btn.png"){
            if(themeP==0){
                theme_btn_path="home/theme_btnBig.png";
            }else{
                theme_btn_path="home1/theme_btnBig.png";
            }
            theme_btn = IMG_LoadTexture(gRenderer, theme_btn_path.c_str());
            theme_btn_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36-4, 330, 88};
        }
    }
    else{
        if(theme_btn_path=="home/theme_btnBig.png" || theme_btn_path=="home1/theme_btnBig.png"){
            if(themeP==0){
                theme_btn_path="home/theme_btn.png";
            }else{
                theme_btn_path="home1/theme_btn.png";
            }
            theme_btn = IMG_LoadTexture(gRenderer, theme_btn_path.c_str());
            theme_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36, 300, 80};
        }
    }

    if(button(background_btn_rect)){
        if(background_btn_path=="home/background_btn.png" || background_btn_path=="home1/background_btn.png"){
            if(themeP==0){
                background_btn_path="home/background_btnBig.png";
            }else{
                background_btn_path="home1/background_btnBig.png";
            }
            background_btn = IMG_LoadTexture(gRenderer, background_btn_path.c_str());
            background_btn_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36*2+80-4, 330, 88};
        }
    }
    else{
        if(background_btn_path=="home/background_btnBig.png" || background_btn_path=="home1/background_btnBig.png"){
            if(themeP==0){
                background_btn_path="home/background_btn.png";
            }else{
                background_btn_path="home1/background_btn.png";
            }
            background_btn = IMG_LoadTexture(gRenderer, background_btn_path.c_str());
            background_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*2+80, 300, 80};
        }
    }

    if(button(ball_btn_rect)){
        if(ball_btn_path=="home/ball_btn.png" || ball_btn_path=="home1/ball_btn.png"){
            if(themeP==0){
                ball_btn_path="home/ball_btnBig.png";
            }else{
                ball_btn_path="home1/ball_btnBig.png";
            }
            ball_btn = IMG_LoadTexture(gRenderer, ball_btn_path.c_str());
            ball_btn_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36*3+80*2-4, 330, 88};
        }
    }
    else{
        if(ball_btn_path=="home/ball_btnBig.png" || ball_btn_path=="home1/ball_btnBig.png"){
            if(themeP==0){
                ball_btn_path="home/ball_btn.png";
            }else{
                ball_btn_path="home1/ball_btn.png";
            }
            ball_btn = IMG_LoadTexture(gRenderer, ball_btn_path.c_str());
            ball_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*3+80*2, 300, 80};
        }
    }

    if(button(cannon_btn_rect)){
        if(cannon_btn_path=="home/cannon_btn.png" || cannon_btn_path=="home1/cannon_btn.png"){
            if(themeP==0){
                cannon_btn_path="home/cannon_btnBig.png";
            }else{
                cannon_btn_path="home1/cannon_btnBig.png";
            }
            cannon_btn = IMG_LoadTexture(gRenderer, cannon_btn_path.c_str());
            cannon_btn_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36*4+80*3-4, 330, 88};
        }
    }
    else{
        if(cannon_btn_path=="home/cannon_btnBig.png" || cannon_btn_path=="home1/cannon_btnBig.png"){
            if(themeP==0){
                cannon_btn_path="home/cannon_btn.png";
            }else{
                cannon_btn_path="home1/cannon_btn.png";
            }
            cannon_btn = IMG_LoadTexture(gRenderer, cannon_btn_path.c_str());
            cannon_btn_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*4+80*3, 300, 80};
        }
    }


    if(button(pacific_rect)){
        if(pacific_path=="home/pacific.png" || pacific_path=="home1/pacific.png"){
            if(themeP==0){
                pacific_path="home/pacificBig.png";
            }else{
                pacific_path="home1/pacificBig.png";
            }
            pacific = IMG_LoadTexture(gRenderer, pacific_path.c_str());
            pacific_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36-4, 330, 88};
        }
    }
    else{
        if(pacific_path=="home/pacificBig.png" || pacific_path=="home1/pacificBig.png"){
            if(themeP==0){
                pacific_path="home/pacific.png";
            }else{
                pacific_path="home1/pacific.png";
            }
            pacific = IMG_LoadTexture(gRenderer, pacific_path.c_str());
            pacific_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36, 300, 80};
        }
    }

    if(button(frozen_rect)){
        if(frozen_path=="home/frozen.png" || frozen_path=="home1/frozen.png"){
            if(themeP==0){
                frozen_path="home/frozenBig.png";
            }else{
                frozen_path="home1/frozenBig.png";
            }
            frozen = IMG_LoadTexture(gRenderer, frozen_path.c_str());
            frozen_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36*2+80-4, 330, 88};
        }
    }
    else{
        if(frozen_path=="home/frozenBig.png" || frozen_path=="home1/frozenBig.png"){
            if(themeP==0){
                frozen_path="home/frozen.png";
            }else{
                frozen_path="home1/frozen.png";
            }
            frozen = IMG_LoadTexture(gRenderer, frozen_path.c_str());
            frozen_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*2+80, 300, 80};
        }
    }

    if(button(safari_rect)){
        if(safari_path=="home/safari.png" || safari_path=="home1/safari.png"){
            if(themeP==0){
                safari_path="home/safariBig.png";
            }else{
                safari_path="home1/safariBig.png";
            }
            safari = IMG_LoadTexture(gRenderer, safari_path.c_str());
            safari_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36*3+80*2-4, 330, 88};
        }
    }
    else{
        if(safari_path=="home/safariBig.png" || safari_path=="home1/safariBig.png"){
            if(themeP==0){
                safari_path="home/safari.png";
            }else{
                safari_path="home1/safari.png";
            }
            safari = IMG_LoadTexture(gRenderer, safari_path.c_str());
            safari_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*3+80*2, 300, 80};
        }
    }

    if(button(jungle_rect)){
        if(jungle_path=="home/jungle.png" || jungle_path=="home1/jungle.png"){
            if(themeP==0){
                jungle_path="home/jungleBig.png";
            }else{
                jungle_path="home1/jungleBig.png";
            }
            jungle = IMG_LoadTexture(gRenderer, jungle_path.c_str());
            jungle_rect = {register_window.x+register_window.w/2-300/2-15, register_window.y+36*4+80*3-4, 330, 88};
        }
    }
    else{
        if(jungle_path=="home/jungleBig.png" || jungle_path=="home1/jungleBig.png"){
            if(themeP==0){
                jungle_path="home/jungle.png";
            }else{
                jungle_path="home1/jungle.png";
            }
            jungle = IMG_LoadTexture(gRenderer, jungle_path.c_str());
            jungle_rect = {register_window.x+register_window.w/2-300/2, register_window.y+36*4+80*3, 300, 80};
        }
    }
}

void home(SDL_Renderer* renderer){
    startTicks = SDL_GetTicks();
    SDL_GetMouseState(&m_x, &m_y);

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            gHome=false;
            quit = true;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            l_press = true;
        }
        if(event.type == SDL_KEYDOWN) {
            //rank scroll
            if(gRanking){
                if(event.key.keysym.sym == SDLK_UP){
                    up=true;
                }
                if(event.key.keysym.sym == SDLK_DOWN){
                    down=true;
                }
            }
        }
    }

    if(Mix_PlayingMusic() == 0){
        if(musicP==0){
            Mix_PlayMusic(frozen_music, -1);
        }
        if(musicP==1){
            Mix_PlayMusic(safari_music, -1);
        }
        if(musicP==2){
            Mix_PlayMusic(pacific_music, -1);
        }
        if(musicP==3){
            Mix_PlayMusic(jungle_music, -1);
        }
    }

    //draw home and effects
    SDL_RenderClear(renderer);
    home_draw(renderer);
    if(!gSetting && !gRanking){
        home_focus();
    }
    //open and draw settings
    if(button(home_setting_rect) && !gSetting && !gRanking && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
        Mix_PlayChannel(-1, toggle_chunk, 0);
        gSetting = true;
        l_press = false;
        if(themeP==0){
            home_setting_path="home/home_setting.png";
        }
        else{
            home_setting_path="home1/home_setting.png";
        }
        home_setting = IMG_LoadTexture(gRenderer, home_setting_path.c_str());
        home_setting_rect = {1060, 134, 100,50};
    }

    if(gSetting){
        volume_rect = {register_window.x+10, chunk_toggle_rect.y, chunk_toggle_rect.x+20-register_window.x, 40};
        SDL_SetRenderDrawColor(renderer, 255,255,000,255);
        SDL_RenderFillRect(renderer, &volume_rect);

        volume_rect = {chunk_toggle_rect.x, chunk_toggle_rect.y, register_window.w+register_window.x-chunk_toggle_rect.x-20, 40};
        SDL_SetRenderDrawColor(renderer, 000,000,000,255);
        SDL_RenderFillRect(renderer, &volume_rect);

        volume_rect = {register_window.x+10, music_toggle_rect.y, music_toggle_rect.x+20-register_window.x, 40};
        SDL_SetRenderDrawColor(renderer, 255,255,000,255);
        SDL_RenderFillRect(renderer, &volume_rect);

        volume_rect = {music_toggle_rect.x, music_toggle_rect.y, register_window.w+register_window.x-music_toggle_rect.x-20, 40};
        SDL_SetRenderDrawColor(renderer, 000,000,000,255);
        SDL_RenderFillRect(renderer, &volume_rect);

        draw_home_setting(renderer);
        if(theme_sec){
            draw_theme(renderer);
            if(button(theme_btn_rect) && theme_sec && !theme_choose_sec && !backg_sec && !ball_sec && !canon_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                theme_choose_sec = true;
                l_press = false;
            }
            if(theme_choose_sec){
                SDL_RenderCopy(renderer, setting_window, NULL, &register_window);
                SDL_RenderCopy(renderer, wood_theme, NULL, &wood_theme_rect);
                SDL_RenderCopy(renderer, metal_theme, NULL, &metal_theme_rect);
                if(button(wood_theme_rect) && !ball_sec && !canon_sec && !backg_sec && theme_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    change_theme(false);
                    themeP=1;
                }
                if(button(metal_theme_rect) && !ball_sec && !canon_sec && theme_sec && !backg_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    change_theme(true);
                    themeP=0;
                }
            }

            if(button(cannon_btn_rect) && theme_sec && !theme_choose_sec && !backg_sec && !ball_sec && !canon_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                canon_sec = true;
                l_press = false;
            }
            if(canon_sec){
                SDL_RenderCopy(renderer, setting_window, NULL, &register_window);
                SDL_RenderCopy(renderer, wood_canon, NULL, &wood_canon_rect);
                SDL_RenderCopy(renderer, metal_canon, NULL, &metal_canon_rect);
                if(button(wood_canon_rect) && canon_sec && !backg_sec && theme_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    cannon = IMG_LoadTexture(renderer, "game/cannon_wood.png");
                    cannonP=1;
                }
                if(button(metal_canon_rect) && canon_sec && theme_sec && !backg_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    cannon = IMG_LoadTexture(renderer, "game/cannon.png");
                    cannonP=0;
                }
            }

            if(button(background_btn_rect) && !theme_choose_sec && theme_sec && !ball_sec && !canon_sec && !music_sec && !backg_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                backg_sec = true;
                l_press = false;
            }
            if(backg_sec){
                SDL_RenderCopy(renderer, setting_window, NULL, &register_window);
                SDL_RenderCopy(renderer, war, NULL, &war_rect);
                SDL_RenderCopy(renderer, peace, NULL, &peace_rect);
                if(button(war_rect) && backg_sec && !canon_sec && theme_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    home_bg = IMG_LoadTexture(renderer, "home/background.png");
                    menu_bg = IMG_LoadTexture(renderer, "menu/background.png");
                    end_bg = IMG_LoadTexture(renderer, "end/background.png");
                    backgroundP=0;
                }
                if(button(peace_rect) && backg_sec && theme_sec && !canon_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    home_bg = IMG_LoadTexture(renderer, "home/background1.png");
                    menu_bg = IMG_LoadTexture(renderer, "menu/background1.png");
                    end_bg = IMG_LoadTexture(renderer, "end/background1.png");
                    backgroundP=1;
                }
            }

            if(button(ball_btn_rect) && !canon_sec && !theme_choose_sec && !backg_sec && theme_sec && !ball_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                ball_sec = true;
                l_press = false;
            }
            if(ball_sec){
                SDL_RenderCopy(renderer, setting_window, NULL, &register_window);
                SDL_RenderCopy(renderer, beach_ball, NULL, &beach_ball_rect);
                SDL_RenderCopy(renderer, reg_ball, NULL, &reg_ball_rect);
                if(button(beach_ball_rect) && ball_sec && !backg_sec && theme_sec && !canon_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    change_ball_color(true);
                    ballP=1;
                }
                if(button(reg_ball_rect) && canon_sec && ball_sec && !backg_sec && theme_sec && !canon_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                    change_ball_color(false);
                    ballP=0;
                }
            }
        }
        if(music_sec){
            draw_music(renderer);
        }
        home_setting_focus();

        if(button(setting_exit_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gSetting = false;
            theme_sec = false;
            music_sec = false;
            canon_sec = false;
            ball_sec = false;
            backg_sec = false;
            theme_choose_sec = false;
            l_press = false;
        }

        if(button(settings_return_rect) && (theme_sec || music_sec) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            theme_sec = false;
            music_sec = false;
            canon_sec = false;
            ball_sec = false;
            backg_sec = false;
            theme_choose_sec = false;
            l_press = false;
        }

        if(button(theme_rect) && !music_sec && !theme_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            theme_sec = true;
            l_press = false;
        }
        if(button(music_rect) && !theme_sec && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            music_sec = true;
            l_press = false;
        }

        if(music_sec){
            if(button(safari_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!safariBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( safari_music, -1 );
                    safariBool = true;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = false;
                    musicP=1;
                }
                l_press = false;
            }
            if(button(frozen_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!frozenBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( frozen_music, -1 );
                    safariBool = false;
                    frozenBool = true;
                    jungleBool = false;
                    pacificBool = false;
                    musicP=0;
                }
                l_press = false;
            }
            if(button(jungle_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!jungleBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( jungle_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = true;
                    pacificBool = false;
                    musicP=3;
                }
                l_press = false;
            }
            if(button(pacific_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!pacificBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( pacific_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = true;
                    musicP=2;
                }
                l_press = false;
            }
        }

        if((button(chunk_toggle_rect) || button(chunk_fissure_rect)) && !music_sec && !theme_sec && l_press){
            if(m_x<=register_window.x+register_window.w-70 && m_x>=register_window.x+70){
                chunk_toggle_rect.x = m_x-20;
                draw_home_setting(renderer);
                if(chunk_toggle_rect.x<410){
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(0)/360);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(0)/360);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(0)/360);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(0)/360);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(0)/360);
                    chunkP=(0);
                }
                else{
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-400)/360);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-400)/360);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-400)/360);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-400)/360);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-400)/360);
                    chunkP=(chunk_toggle_rect.x-400);
                }
            }
        }
        else if(!(button(music_toggle_rect) || button(music_fissure_rect)) && !music_sec && !theme_sec){
            l_press = false;
        }

        if((button(music_toggle_rect) || button(music_fissure_rect)) && l_press && !music_sec && !theme_sec){
            if(m_x<=register_window.x+register_window.w-70 && m_x>=register_window.x+70){
                music_toggle_rect.x = m_x-20;
                draw_home_setting(renderer);
                if(music_toggle_rect.x<410){
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(0)/360);
                    soundP=(0);
                }
                else{
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(music_toggle_rect.x-400)/360);
                    soundP=(music_toggle_rect.x-400);
                }
            }
        }
        else if(!(button(chunk_toggle_rect) || button(chunk_fissure_rect)) && !music_sec && !theme_sec){
            l_press = false;
        }
    }

    //rankings
    if(button(ranking_rect) && !gRanking && !gSetting && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
        Mix_PlayChannel(-1, toggle_chunk, 0);
        gRanking = true;
        l_press = false;
        ranking_path="home/ranking.png";
        ranking = IMG_LoadTexture(gRenderer, ranking_path.c_str());
        ranking_rect = {width/2-330/2, 570, 330, 62};
    }

    if(gRanking){
        if(once){
            pull_db();
            compare(0);
            once = false;
        }
        if(up){
            rank_scroll(true);
            up = false;
        }
        if(down){
            rank_scroll(false);
            down = false;
        }
        if(button(norm_header) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            compare(0);
            l_press = false;
        }
        if(button(inf_header) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            compare(1);
            l_press = false;
        }
        if(button(time_header) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            compare(2);
            l_press = false;
        }
        if(button(hard_header) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            compare(3);
            l_press = false;
        }
        if(button(overall_header) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            compare(4);
            l_press = false;
        }
        if(button(ranking_exit_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gRanking = false;
            l_press = false;
        }
        draw_ranking(renderer);
    }
    else{
        once=true;
    }

    //go to menu
    if(button(start_rect) && !gSetting && !gRanking && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
        Mix_PlayChannel(-1, toggle_chunk, 0);
        gHome = false;
        gMenu = true;
        once = true;
        l_press = false;
        return;
    }

    elapsedTicks = SDL_GetTicks() - startTicks;
    while (elapsedTicks < desiredDelta) {
        elapsedTicks = SDL_GetTicks() - startTicks;
    }
    SDL_RenderPresent(renderer);
}

void draw_menu(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer, menu_bg, NULL, &menu_bg_rect);
    SDL_RenderCopy(renderer, menu_header, NULL, &menu_header_rect);
    SDL_RenderCopy(renderer, player_header, NULL, &player_header_rect);
    SDL_RenderCopy(renderer, menu_return, NULL, &menu_return_rect);
    SDL_RenderCopy(renderer, shop, NULL, &shop_rect);

    layer_rect={normal_mode_rect.x+15, normal_mode_rect.y+108, 50, 50};
    SDL_RenderCopy(renderer, normal_mode, NULL, &normal_mode_rect);
    SDL_RenderCopy(renderer, infinite_mode, NULL, &infinite_mode_rect);
    SDL_RenderCopy(renderer, timer_mode, NULL, &timer_mode_rect);
    SDL_RenderCopy(renderer, hardcore_mode, NULL, &hardcore_mode_rect);

    if(norm_lv == 1){
        if(norm_button1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, button1, NULL, &norm_button1_rect);
        }
        else{
            SDL_RenderCopy(renderer, button1Big, NULL, &norm_button1_rect);
        }

        if(norm_lock1_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, lock1, NULL, &norm_lock1_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock1Big, NULL, &norm_lock1_rect);
        }

        if(norm_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &norm_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &norm_lock2_rect);
        }
    }
    if(norm_lv == 2){
        if(norm_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &norm_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &norm_medal1_rect);
        }

        if(norm_button2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, button2, NULL, &norm_button2_rect);
        }
        else{
            SDL_RenderCopy(renderer, button2Big, NULL, &norm_button2_rect);
        }

        if(norm_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &norm_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &norm_lock2_rect);
        }
    }
    if(norm_lv == 3){
        if(norm_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &norm_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &norm_medal1_rect);
        }

        if(norm_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &norm_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &norm_medal2_rect);
        }

        if(norm_button3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, button3, NULL, &norm_button3_rect);
        }
        else{
            SDL_RenderCopy(renderer, button3Big, NULL, &norm_button3_rect);
        }
    }
    if(norm_lv == 4){
        if(norm_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &norm_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &norm_medal1_rect);
        }

        if(norm_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &norm_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &norm_medal2_rect);
        }

        if(norm_medal3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, medal3, NULL, &norm_medal3_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal3Big, NULL, &norm_medal3_rect);
        }
    }

    if(inf_lv == 1){
        if(inf_button1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, button1, NULL, &inf_button1_rect);
        }
        else{
            SDL_RenderCopy(renderer, button1Big, NULL, &inf_button1_rect);
        }

        if(inf_lock1_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, lock1, NULL, &inf_lock1_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock1Big, NULL, &inf_lock1_rect);
        }

        if(inf_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &inf_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &inf_lock2_rect);
        }
    }
    if(inf_lv == 2){
        if(inf_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &inf_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &inf_medal1_rect);
        }

        if(inf_button2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, button2, NULL, &inf_button2_rect);
        }
        else{
            SDL_RenderCopy(renderer, button2Big, NULL, &inf_button2_rect);
        }

        if(inf_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &inf_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &inf_lock2_rect);
        }
    }
    if(inf_lv == 3){
        if(inf_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &inf_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &inf_medal1_rect);
        }

        if(inf_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &inf_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &inf_medal2_rect);
        }

        if(inf_button3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, button3, NULL, &inf_button3_rect);
        }
        else{
            SDL_RenderCopy(renderer, button3Big, NULL, &inf_button3_rect);
        }
    }
    if(inf_lv == 4){
        if(inf_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &inf_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &inf_medal1_rect);
        }

        if(inf_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &inf_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &inf_medal2_rect);
        }

        if(inf_medal3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, medal3, NULL, &inf_medal3_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal3Big, NULL, &inf_medal3_rect);
        }
    }

    if(time_lv == 1){
        if(time_button1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, button1, NULL, &time_button1_rect);
        }
        else{
            SDL_RenderCopy(renderer, button1Big, NULL, &time_button1_rect);
        }

        if(time_lock1_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, lock1, NULL, &time_lock1_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock1Big, NULL, &time_lock1_rect);
        }

        if(time_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &time_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &time_lock2_rect);
        }
    }
    if(time_lv == 2){
        if(time_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &time_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &time_medal1_rect);
        }

        if(time_button2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, button2, NULL, &time_button2_rect);
        }
        else{
            SDL_RenderCopy(renderer, button2Big, NULL, &time_button2_rect);
        }

        if(time_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &time_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &time_lock2_rect);
        }
    }
    if(time_lv == 3){
        if(time_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &time_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &time_medal1_rect);
        }

        if(time_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &time_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &time_medal2_rect);
        }

        if(time_button3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, button3, NULL, &time_button3_rect);
        }
        else{
            SDL_RenderCopy(renderer, button3Big, NULL, &time_button3_rect);
        }
    }
    if(time_lv == 4){
        if(time_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &time_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &time_medal1_rect);
        }

        if(time_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &time_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &time_medal2_rect);
        }

        if(time_medal3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, medal3, NULL, &time_medal3_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal3Big, NULL, &time_medal3_rect);
        }
    }

    if(hardcore_lv == 1){
        if(hard_button1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, button1, NULL, &hard_button1_rect);
        }
        else{
            SDL_RenderCopy(renderer, button1Big, NULL, &hard_button1_rect);
        }

        if(hard_lock1_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, lock1, NULL, &hard_lock1_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock1Big, NULL, &hard_lock1_rect);
        }

        if(hard_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &hard_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &hard_lock2_rect);
        }
    }
    if(hardcore_lv == 2){
        if(hard_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &hard_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &hard_medal1_rect);
        }

        if(hard_button2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, button2, NULL, &hard_button2_rect);
        }
        else{
            SDL_RenderCopy(renderer, button2Big, NULL, &hard_button2_rect);
        }

        if(hard_lock2_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, lock2, NULL, &hard_lock2_rect);
        }
        else{
            SDL_RenderCopy(renderer, lock2Big, NULL, &hard_lock2_rect);
        }
    }
    if(hardcore_lv == 3){
        if(hard_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &hard_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &hard_medal1_rect);
        }

        if(hard_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &hard_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &hard_medal2_rect);
        }

        if(hard_button3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, button3, NULL, &hard_button3_rect);
        }
        else{
            SDL_RenderCopy(renderer, button3Big, NULL, &hard_button3_rect);
        }
    }
    if(hardcore_lv == 4){
        if(hard_medal1_rect.x == normal_mode_rect.x+90){
            SDL_RenderCopy(renderer, medal1, NULL, &hard_medal1_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal1Big, NULL, &hard_medal1_rect);
        }

        if(hard_medal2_rect.x == normal_mode_rect.x+90+211){
            SDL_RenderCopy(renderer, medal2, NULL, &hard_medal2_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal2Big, NULL, &hard_medal2_rect);
        }

        if(hard_medal3_rect.x == normal_mode_rect.x+90+211*2){
            SDL_RenderCopy(renderer, medal3, NULL, &hard_medal3_rect);
        }
        else{
            SDL_RenderCopy(renderer, medal3Big, NULL, &hard_medal3_rect);
        }
    }

    point_rect = {0, (height-normal_mode_rect.y-normal_mode_rect.h)/2-10+(normal_mode_rect.y+normal_mode_rect.h), 21, 21};
    for(int i = 0; i < 4; i++){
        point_rect.x = 498 + i*51;
        SDL_RenderCopy(renderer, point, NULL, &point_rect);
    }

    if(normal_mode_rect.x>0 && normal_mode_rect.x+normal_mode_rect.w<width){
        point_rect = {498-3, point_rect.y-3, 27, 27};
        SDL_RenderCopy(renderer, pointBig, NULL, &point_rect);
    }
    if(infinite_mode_rect.x>0 && infinite_mode_rect.x+infinite_mode_rect.w<width){
        point_rect = {498+51-3, point_rect.y-3, 27, 27};
        SDL_RenderCopy(renderer, pointBig, NULL, &point_rect);
    }
    if(timer_mode_rect.x>0 && timer_mode_rect.x+timer_mode_rect.w<width){
        point_rect = {498+51*2-3, point_rect.y-3, 27, 27};
        SDL_RenderCopy(renderer, pointBig, NULL, &point_rect);
    }
    if(hardcore_mode_rect.x>0 && hardcore_mode_rect.x+hardcore_mode_rect.w<width){
        point_rect = {498+51*3-3, point_rect.y-3, 27, 27};
        SDL_RenderCopy(renderer, pointBig, NULL, &point_rect);
    }

    if(open_register){
        text(renderer, player_header_rect.x+50, player_header_rect.y+4, "Player", 255, 255, 255, 255, 1);
    }
    else{
        text(renderer, player_header_rect.x+50, player_header_rect.y+4, user.c_str(), 255, 255, 255, 255, 1);
    }
}

void menu_scroll(bool r){
    if(r && hardcore_mode_rect.x>770){
        normal_mode_rect.x-=770;
        infinite_mode_rect.x-=770;
        timer_mode_rect.x-=770;
        hardcore_mode_rect.x-=770;
    }
    else if(!r && normal_mode_rect.x<0){
        normal_mode_rect.x+=770;
        infinite_mode_rect.x+=770;
        timer_mode_rect.x+=770;
        hardcore_mode_rect.x+=770;
    }
    norm_button1_rect = {normal_mode_rect.x+90, normal_mode_rect.y+257, 119, 147};
    norm_button2_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 147};
    norm_button3_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 147};
    norm_lock1_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 147};
    norm_lock2_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 147};
    norm_medal1_rect = {normal_mode_rect.x+90, normal_mode_rect.y+257, 119, 159};
    norm_medal2_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 159};
    norm_medal3_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 158};

    inf_button1_rect = {infinite_mode_rect.x+90, infinite_mode_rect.y+257, 119, 147};
    inf_button2_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 147};
    inf_button3_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 147};
    inf_lock1_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 147};
    inf_lock2_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 147};
    inf_medal1_rect = {infinite_mode_rect.x+90, infinite_mode_rect.y+257, 119, 159};
    inf_medal2_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 159};
    inf_medal3_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 158};

    time_button1_rect = {timer_mode_rect.x+90, timer_mode_rect.y+257, 119, 147};
    time_button2_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 147};
    time_button3_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 147};
    time_lock1_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 147};
    time_lock2_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 147};
    time_medal1_rect = {timer_mode_rect.x+90, timer_mode_rect.y+257, 119, 159};
    time_medal2_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 159};
    time_medal3_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 158};

    hard_button1_rect = {hardcore_mode_rect.x+90, hardcore_mode_rect.y+257, 119, 147};
    hard_button2_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 147};
    hard_button3_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 147};
    hard_lock1_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 147};
    hard_lock2_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 147};
    hard_medal1_rect = {hardcore_mode_rect.x+90, hardcore_mode_rect.y+257, 119, 159};
    hard_medal2_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 159};
    hard_medal3_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 158};
}

void menu_focus(){
    if(button(login_rect)){
        if(login_btn_path=="menu/login.png" || login_btn_path=="menu1/login.png"){
            if(themeP==0){
                login_btn_path="menu/loginBig.png";
            }else{
                login_btn_path="menu1/loginBig.png";
            }
            login_btn = IMG_LoadTexture(gRenderer, login_btn_path.c_str());
            login_rect = {register_window.x+register_window.w/2-235/2-11, pass_rect.y+pass_rect.h+50-3, 258, 70};
        }
    }
    else{
        if(login_btn_path=="menu/loginBig.png" || login_btn_path=="menu1/loginBig.png"){
            if(themeP==0){
                login_btn_path="menu/login.png";
            }else{
                login_btn_path="menu1/login.png";
            }
            login_btn = IMG_LoadTexture(gRenderer, login_btn_path.c_str());
            login_rect = {register_window.x+register_window.w/2-235/2, pass_rect.y+pass_rect.h+50, 235, 64};
        }
    }

    if(button(register_rect)){
        if(register_btn_path=="menu/register.png" || register_btn_path=="menu1/register.png"){
            if(themeP==0){
                register_btn_path="menu/registerBig.png";
            }else{
                register_btn_path="menu1/registerBig.png";
            }
            register_btn = IMG_LoadTexture(gRenderer, register_btn_path.c_str());
            register_rect = {register_window.x+register_window.w/2-235/2-11, login_rect.y+login_rect.h+30-3, 258, 70};
        }
    }
    else{
        if(register_btn_path=="menu/registerBig.png" || register_btn_path=="menu1/registerBig.png"){
            if(themeP==0){
                register_btn_path="menu/register.png";
            }else{
                register_btn_path="menu1/register.png";
            }
            register_btn = IMG_LoadTexture(gRenderer, register_btn_path.c_str());
            register_rect = {register_window.x+register_window.w/2-235/2, login_rect.y+login_rect.h+30, 235, 64};
        }
    }

    if(show_register){
        return;
    }

    if(button(menu_return_rect)){
        if(menu_return_path=="menu/return.png" || menu_return_path=="menu1/return.png"){
            if(themeP==0){
                menu_return_path="menu/returnBig.png";
            }else{
                menu_return_path="menu1/returnBig.png";
            }
            menu_return = IMG_LoadTexture(gRenderer, menu_return_path.c_str());
            menu_return_rect = {30-7, 30-4, 115, 57};
        }
    }
    else{
        if(menu_return_path=="menu/returnBig.png" || menu_return_path=="menu1/returnBig.png"){
            if(themeP==0){
                menu_return_path="menu/return.png";
            }else{
                menu_return_path="menu1/return.png";
            }
            menu_return = IMG_LoadTexture(gRenderer, menu_return_path.c_str());
            menu_return_rect = {30, 30, 100, 50};
        }
    }

    if(button(shop_rect)){
        if(shop_path=="menu/shop.png" || shop_path=="menu1/shop.png"){
            if(themeP==0){
                shop_path="menu/shopBig.png";
            }else{
                shop_path="menu1/shopBig.png";
            }
            shop = IMG_LoadTexture(gRenderer, shop_path.c_str());
            shop_rect = {1070-7, 30-4, 115, 57};
        }
    }
    else{
        if(shop_path=="menu/shopBig.png" || shop_path=="menu1/shopBig.png"){
            if(themeP==0){
                shop_path="menu/shop.png";
            }else{
                shop_path="menu1/shop.png";
            }
            shop = IMG_LoadTexture(gRenderer, shop_path.c_str());
            shop_rect = {1070, 30, 100, 50};
        }
    }

    if(button(norm_button1_rect)){
        if(norm_button1_rect.x == normal_mode_rect.x+90){
            norm_button1_rect = {normal_mode_rect.x+90-9, normal_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(norm_button1_rect.x == normal_mode_rect.x+90-9){
            norm_button1_rect = {normal_mode_rect.x+90, normal_mode_rect.y+257, 119, 147};
        }
    }

    if(button(norm_button2_rect)){
        if(norm_button2_rect.x == normal_mode_rect.x+90+211){
            norm_button2_rect = {normal_mode_rect.x+90+211-9, normal_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(norm_button2_rect.x == normal_mode_rect.x+90+211-9){
            norm_button2_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 147};
        }
    }

    if(button(norm_button3_rect)){
        if(norm_button3_rect.x == normal_mode_rect.x+90+211*2){
            norm_button3_rect = {normal_mode_rect.x+90+211*2-9, normal_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(norm_button3_rect.x == normal_mode_rect.x+90+211*2-9){
            norm_button3_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 147};
        }
    }

    if(button(norm_lock1_rect)){
        if(norm_lock1_rect.x == normal_mode_rect.x+90+211){
            norm_lock1_rect = {normal_mode_rect.x+90+211-9, normal_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(norm_lock1_rect.x == normal_mode_rect.x+90+211-9){
            norm_lock1_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 147};
        }
    }

    if(button(norm_lock2_rect)){
        if(norm_lock2_rect.x == normal_mode_rect.x+90+211*2){
            norm_lock2_rect = {normal_mode_rect.x+90+211*2-9, normal_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(norm_lock2_rect.x == normal_mode_rect.x+90+211*2-9){
            norm_lock2_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 147};
        }
    }

    if(button(norm_medal1_rect)){
        if(norm_medal1_rect.x == normal_mode_rect.x+90){
            norm_medal1_rect = {normal_mode_rect.x+90-9, normal_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(norm_medal1_rect.x == normal_mode_rect.x+90-9){
            norm_medal1_rect = {normal_mode_rect.x+90, normal_mode_rect.y+257, 119, 159};
        }
    }

    if(button(norm_medal2_rect)){
        if(norm_medal2_rect.x == normal_mode_rect.x+90+211){
            norm_medal2_rect = {normal_mode_rect.x+90+211-9, normal_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(norm_medal2_rect.x == normal_mode_rect.x+90+211-9){
            norm_medal2_rect = {normal_mode_rect.x+90+211, normal_mode_rect.y+257, 119, 159};
        }
    }

    if(button(norm_medal3_rect)){
        if(norm_medal3_rect.x == normal_mode_rect.x+90+211*2){
            norm_medal3_rect = {normal_mode_rect.x+90+211*2-9, normal_mode_rect.y+257-6, 137, 182};
        }
    }
    else{
        if(norm_medal3_rect.x == normal_mode_rect.x+90+211*2-9){
            norm_medal3_rect = {normal_mode_rect.x+90+211*2, normal_mode_rect.y+257, 119, 158};
        }
    }

    if(button(inf_button1_rect)){
        if(inf_button1_rect.x == infinite_mode_rect.x+90){
            inf_button1_rect = {infinite_mode_rect.x+90-9, infinite_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(inf_button1_rect.x == infinite_mode_rect.x+90-9){
            inf_button1_rect = {infinite_mode_rect.x+90, infinite_mode_rect.y+257, 119, 147};
        }
    }

    if(button(inf_button2_rect)){
        if(inf_button2_rect.x == infinite_mode_rect.x+90+211){
            inf_button2_rect = {infinite_mode_rect.x+90+211-9, infinite_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(inf_button2_rect.x == infinite_mode_rect.x+90+211-9){
            inf_button2_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 147};
        }
    }

    if(button(inf_button3_rect)){
        if(inf_button3_rect.x == infinite_mode_rect.x+90+211*2){
            inf_button3_rect = {infinite_mode_rect.x+90+211*2-9, infinite_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(inf_button3_rect.x == infinite_mode_rect.x+90+211*2-9){
            inf_button3_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 147};
        }
    }

    if(button(inf_lock1_rect)){
        if(inf_lock1_rect.x == infinite_mode_rect.x+90+211){
            inf_lock1_rect = {infinite_mode_rect.x+90+211-9, infinite_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(inf_lock1_rect.x == infinite_mode_rect.x+90+211-9){
            inf_lock1_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 147};
        }
    }

    if(button(inf_lock2_rect)){
        if(inf_lock2_rect.x == infinite_mode_rect.x+90+211*2){
            inf_lock2_rect = {infinite_mode_rect.x+90+211*2-9, infinite_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(inf_lock2_rect.x == infinite_mode_rect.x+90+211*2-9){
            inf_lock2_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 147};
        }
    }

    if(button(inf_medal1_rect)){
        if(inf_medal1_rect.x == infinite_mode_rect.x+90){
            inf_medal1_rect = {infinite_mode_rect.x+90-9, infinite_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(inf_medal1_rect.x == infinite_mode_rect.x+90-9){
            inf_medal1_rect = {infinite_mode_rect.x+90, infinite_mode_rect.y+257, 119, 159};
        }
    }

    if(button(inf_medal2_rect)){
        if(inf_medal2_rect.x == infinite_mode_rect.x+90+211){
            inf_medal2_rect = {infinite_mode_rect.x+90+211-9, infinite_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(inf_medal2_rect.x == infinite_mode_rect.x+90+211-9){
            inf_medal2_rect = {infinite_mode_rect.x+90+211, infinite_mode_rect.y+257, 119, 159};
        }
    }

    if(button(inf_medal3_rect)){
        if(inf_medal3_rect.x == infinite_mode_rect.x+90+211*2){
            inf_medal3_rect = {infinite_mode_rect.x+90+211*2-9, infinite_mode_rect.y+257-6, 137, 182};
        }
    }
    else{
        if(inf_medal3_rect.x == normal_mode_rect.x+90+211*2-9){
            inf_medal3_rect = {infinite_mode_rect.x+90+211*2, infinite_mode_rect.y+257, 119, 158};
        }
    }

    if(button(time_button1_rect)){
        if(time_button1_rect.x == timer_mode_rect.x+90){
            time_button1_rect = {timer_mode_rect.x+90-9, timer_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(time_button1_rect.x == timer_mode_rect.x+90-9){
            time_button1_rect = {timer_mode_rect.x+90, timer_mode_rect.y+257, 119, 147};
        }
    }

    if(button(time_button2_rect)){
        if(time_button2_rect.x == timer_mode_rect.x+90+211){
            time_button2_rect = {timer_mode_rect.x+90+211-9, timer_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(time_button2_rect.x == timer_mode_rect.x+90+211-9){
            time_button2_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 147};
        }
    }

    if(button(time_button3_rect)){
        if(time_button3_rect.x == timer_mode_rect.x+90+211*2){
            time_button3_rect = {timer_mode_rect.x+90+211*2-9, timer_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(time_button3_rect.x == timer_mode_rect.x+90+211*2-9){
            time_button3_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 147};
        }
    }

    if(button(time_lock1_rect)){
        if(time_lock1_rect.x == timer_mode_rect.x+90+211){
            time_lock1_rect = {timer_mode_rect.x+90+211-9, timer_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(time_lock1_rect.x == timer_mode_rect.x+90+211-9){
            time_lock1_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 147};
        }
    }

    if(button(time_lock2_rect)){
        if(time_lock2_rect.x == timer_mode_rect.x+90+211*2){
            time_lock2_rect = {timer_mode_rect.x+90+211*2-9, timer_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(time_lock2_rect.x == timer_mode_rect.x+90+211*2-9){
            time_lock2_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 147};
        }
    }

    if(button(time_medal1_rect)){
        if(time_medal1_rect.x == timer_mode_rect.x+90){
            time_medal1_rect = {timer_mode_rect.x+90-9, timer_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(time_medal1_rect.x == timer_mode_rect.x+90-9){
            time_medal1_rect = {timer_mode_rect.x+90, timer_mode_rect.y+257, 119, 159};
        }
    }

    if(button(time_medal2_rect)){
        if(time_medal2_rect.x == timer_mode_rect.x+90+211){
            time_medal2_rect = {timer_mode_rect.x+90+211-9, timer_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(time_medal2_rect.x == timer_mode_rect.x+90+211-9){
            time_medal2_rect = {timer_mode_rect.x+90+211, timer_mode_rect.y+257, 119, 159};
        }
    }

    if(button(time_medal3_rect)){
        if(time_medal3_rect.x == timer_mode_rect.x+90+211*2){
            time_medal3_rect = {timer_mode_rect.x+90+211*2-9, timer_mode_rect.y+257-6, 137, 182};
        }
    }
    else{
        if(time_medal3_rect.x == normal_mode_rect.x+90+211*2-9){
            time_medal3_rect = {timer_mode_rect.x+90+211*2, timer_mode_rect.y+257, 119, 158};
        }
    }

    if(button(hard_button1_rect)){
        if(hard_button1_rect.x == hardcore_mode_rect.x+90){
            hard_button1_rect = {hardcore_mode_rect.x+90-9, hardcore_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(hard_button1_rect.x == hardcore_mode_rect.x+90-9){
            hard_button1_rect = {hardcore_mode_rect.x+90, hardcore_mode_rect.y+257, 119, 147};
        }
    }

    if(button(hard_button2_rect)){
        if(hard_button2_rect.x == hardcore_mode_rect.x+90+211){
            hard_button2_rect = {hardcore_mode_rect.x+90+211-9, hardcore_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(hard_button2_rect.x == hardcore_mode_rect.x+90+211-9){
            hard_button2_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 147};
        }
    }

    if(button(hard_button3_rect)){
        if(hard_button3_rect.x == hardcore_mode_rect.x+90+211*2){
            hard_button3_rect = {hardcore_mode_rect.x+90+211*2-9, hardcore_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(hard_button3_rect.x == hardcore_mode_rect.x+90+211*2-9){
            hard_button3_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 147};
        }
    }

    if(button(hard_lock1_rect)){
        if(hard_lock1_rect.x == hardcore_mode_rect.x+90+211){
            hard_lock1_rect = {hardcore_mode_rect.x+90+211-9, hardcore_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(hard_lock1_rect.x == hardcore_mode_rect.x+90+211-9){
            hard_lock1_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 147};
        }
    }

    if(button(hard_lock2_rect)){
        if(hard_lock2_rect.x == hardcore_mode_rect.x+90+211*2){
            hard_lock2_rect = {hardcore_mode_rect.x+90+211*2-9, hardcore_mode_rect.y+257-11, 137, 170};
        }
    }
    else{
        if(hard_lock2_rect.x == hardcore_mode_rect.x+90+211*2-9){
            hard_lock2_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 147};
        }
    }

    if(button(hard_medal1_rect)){
        if(hard_medal1_rect.x == hardcore_mode_rect.x+90){
            hard_medal1_rect = {hardcore_mode_rect.x+90-9, hardcore_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(hard_medal1_rect.x == hardcore_mode_rect.x+90-9){
            hard_medal1_rect = {hardcore_mode_rect.x+90, hardcore_mode_rect.y+257, 119, 159};
        }
    }

    if(button(hard_medal2_rect)){
        if(hard_medal2_rect.x == hardcore_mode_rect.x+90+211){
            hard_medal2_rect = {hardcore_mode_rect.x+90+211-9, hardcore_mode_rect.y+257-6, 137, 183};
        }
    }
    else{
        if(hard_medal2_rect.x == hardcore_mode_rect.x+90+211-9){
            hard_medal2_rect = {hardcore_mode_rect.x+90+211, hardcore_mode_rect.y+257, 119, 159};
        }
    }

    if(button(hard_medal3_rect)){
        if(hard_medal3_rect.x == hardcore_mode_rect.x+90+211*2){
            hard_medal3_rect = {hardcore_mode_rect.x+90+211*2-9, hardcore_mode_rect.y+257-6, 137, 182};
        }
    }
    else{
        if(hard_medal3_rect.x == hardcore_mode_rect.x+90+211*2-9){
            hard_medal3_rect = {hardcore_mode_rect.x+90+211*2, hardcore_mode_rect.y+257, 119, 158};
        }
    }
}

void menu(SDL_Renderer* renderer){
    startTicks = SDL_GetTicks();
    SDL_GetMouseState(&m_x, &m_y);

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            gMenu = false;
            quit = true;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            l_press = true;
        }
        if(event.type == SDL_KEYDOWN){
            //register stuff
            if(open_register && show_register){
                if (event.key.keysym.sym == SDLK_RETURN  && !last) {
                    last = true;
                }

                if (event.key.keysym.sym == SDLK_BACKSPACE && !last && user.length() > 0) {
                    user.pop_back();
                }

                if (event.key.keysym.sym == SDLK_BACKSPACE && last && pass.length() > 0) {
                    pass.pop_back();
                }
            }

            //mode scroll
            if(event.key.keysym.sym == SDLK_RIGHT){
                t_right = true;
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                t_left = true;
            }
        }
        //register stuff
        if(event.type == SDL_TEXTINPUT && open_register && show_register){
            if(!last){
                user += event.text.text;
                if(user.length() > 100){
                    user.pop_back();
                }
            }
            else{
                pass += event.text.text;
                if(pass.length() > 100){
                    pass.pop_back();
                }
            }
        }
    }

    SDL_RenderClear(renderer);
    draw_menu(renderer);

    menu_focus();

    if(button(menu_return_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
        Mix_PlayChannel(-1, toggle_chunk, 0);
        gMenu = false;
        gHome = true;
        once = true;
        l_press = false;
        return;
    }
    if(t_right){
        menu_scroll(true);
        t_right = false;
    }
    if(t_left){
        menu_scroll(false);
        t_left = false;
    }

    if(button(normal_mode_rect)){
        if(norm_lv == 1){
            if(button(norm_button1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(norm_button1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(norm_lv == 2){
            if(button(norm_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(norm_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(norm_button2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(norm_button2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(norm_lv == 3){
            if(button(norm_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(norm_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(norm_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(open_register && !show_register){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(norm_button3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(norm_button3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(norm_lv == 4){
            if(button(norm_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(norm_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(norm_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(norm_medal2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(norm_medal3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gNormal = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(norm_medal3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
    }

    if(button(infinite_mode_rect)){
        if(inf_lv == 1){
            if(button(inf_button1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(inf_button1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(inf_lv == 2){
            if(button(inf_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(inf_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(inf_button2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(inf_button2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(inf_lv == 3){
            if(button(inf_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(inf_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(inf_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(inf_medal2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(inf_button3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(inf_button3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(inf_lv == 4){
            if(button(inf_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(inf_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(inf_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(inf_medal2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(inf_medal3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gInfinite = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(inf_medal3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
    }

    if(button(timer_mode_rect)){
        if(time_lv == 1){
            if(button(time_button1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(time_button1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(time_lv == 2){
            if(button(time_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(time_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(time_button2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(time_button2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(time_lv == 3){
            if(button(time_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(time_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(time_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(time_medal2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(time_button3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(time_button3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(time_lv == 4){
            if(button(time_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(time_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(time_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(time_medal2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(time_medal3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gTimer = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(time_medal3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
    }

    if(button(hardcore_mode_rect)){
        if(hardcore_lv == 1){
            if(button(hard_button1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(hard_button1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(hardcore_lv == 2){
            if(button(hard_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(hard_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(hard_button2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(hard_button2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(hardcore_lv == 3){
            if(button(hard_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(hard_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(hard_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                mid = true;
                once = true;
                l_press = false;
            }else if(button(hard_medal2_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(hard_button3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(hard_button3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
        if(hardcore_lv == 4){
            if(button(hard_medal1_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                intro = true;
                once = true;
                l_press = false;
            }else if(button(hard_medal1_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(hard_medal2_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                mid = true;
                once = true;
                l_press = false;
            }
            else if(button(hard_medal2_rect) && open_register && !show_register &&
                    event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }

            if(button(hard_medal3_rect) && !open_register && !show_register &&
               event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_HaltMusic();
                gMenu = false;
                gHardcore = true;
                pro = true;
                once = true;
                l_press = false;
            }else if(button(hard_medal3_rect) && open_register && !show_register &&
                     event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                Mix_PlayChannel(-1, toggle_chunk, 0);
                show_register = true;
                l_press = false;
            }
        }
    }

    if(show_register && open_register){
        draw_register(renderer);
        text_writer(renderer);

        if(button(register_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            input = true;
            new_player = true;
            l_press = false;
        }
        if(button(login_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            input = true;
            new_player = false;
            l_press = false;
        }
        if(button(register_exit_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            show_register = false;
            user="";
            pass="";
            last = false;
            l_press = false;
        }

        if(error_reg){
            text(renderer, error_rect.x, error_rect.y, wrong_reg.c_str(), 255, 000, 000, 255, 0);
        }
        if(error_log){
            text(renderer, error_rect.x, error_rect.y, wrong_log.c_str(), 255, 000, 000, 255, 0);
        }
        if((error_log || error_reg) && user.length()!=0){
            error_reg=false;
            error_log=false;
        }
    }

    elapsedTicks = SDL_GetTicks() - startTicks;
    while (elapsedTicks < desiredDelta) {
        elapsedTicks = SDL_GetTicks() - startTicks;
    }
    SDL_RenderPresent(renderer);
}

void end_focus(){
    if(button(to_home_rect)){
        if(to_home_path=="end/to_home.png" || to_home_path=="end1/to_home.png"){
            if(themeP==0){
                to_home_path="end/to_homeBig.png";
            }else{
                to_home_path="end1/to_homeBig.png";
            }
            to_home = IMG_LoadTexture(gRenderer, to_home_path.c_str());
            to_home_rect = {30-7, 300-4, 115, 57};

        }
    }
    else{
        if(to_home_path=="end/to_homeBig.png" || to_home_path=="end1/to_home.png"){
            if(themeP==0){
                to_home_path="end/to_home.png";
            }else{
                to_home_path="end1/to_home.png";
            }
            to_home = IMG_LoadTexture(gRenderer, to_home_path.c_str());
            to_home_rect = {30, 300, 100, 50};
        }
    }

    if(button(to_menu_rect)){
        if(to_menu_path=="end/to_menu.png" || to_menu_path=="end1/to_menu.png"){
            if(themeP==0){
                to_menu_path="end/to_menuBig.png";
            }else{
                to_menu_path="end1/to_menuBig.png";
            }
            to_menu = IMG_LoadTexture(gRenderer, to_menu_path.c_str());
            to_menu_rect = {1070-7, 300-4, 115, 57};
        }
    }
    else{
        if(to_menu_path=="end/to_menuBig.png" || to_menu_path=="end1/to_menu.png"){
            if(themeP==0){
                to_menu_path="end/to_menu.png";
            }else{
                to_menu_path="end1/to_menu.png";
            }
            to_menu = IMG_LoadTexture(gRenderer, to_menu_path.c_str());
            to_menu_rect = {1070, 300, 100, 50};
        }
    }

    if(button(replay_rect)){
        if(replay_path=="end/replay.png" || replay_path=="end1/replay.png"){
            if(themeP==0){
                replay_path="end/replayBig.png";
            }else{
                replay_path="end1/replayBig.png";
            }
            replay = IMG_LoadTexture(gRenderer, replay_path.c_str());
            replay_rect = {1070-7, 380-4, 115, 57};
        }
    }
    else{
        if(replay_path=="end/replayBig.png" || replay_path=="end1/replay.png"){
            if(themeP==0){
                replay_path="end/replay.png";
            }else{
                replay_path="end1/replay.png";
            }
            replay = IMG_LoadTexture(gRenderer, replay_path.c_str());
            replay_rect = {1070, 380, 100, 50};
        }
    }
}

void end(SDL_Renderer *renderer, int k){
    startTicks = SDL_GetTicks();

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            gEnd = false;
            quit = true;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            l_press = true;
        }
    }
    SDL_GetMouseState(&m_x, &m_y);

    bool stop = false;

    SDL_SetRenderDrawColor(renderer, 000,000,000,255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, end_bg, NULL, &end_bg_rect);
    SDL_RenderCopy(renderer, end_window, NULL, &end_window_rect);
    SDL_RenderCopy(renderer, to_home, NULL, &to_home_rect);
    SDL_RenderCopy(renderer, to_menu, NULL, &to_menu_rect);
    SDL_RenderCopy(renderer, replay, NULL, &replay_rect);
    SDL_Rect user_end_rect = {width/2-260/2+95 ,18, 260, 40};
    text(renderer, user_end_rect.x, user_end_rect.y, user.c_str(), 255, 255, 255, 255, 1);
    if(win){
        if(once){
            Mix_PlayMusic( game_win_music, 0);
            once = false;
            if(mode==1){
                if(intro){
                    if(norm_lv==1){
                        norm_lv=2;
                    }
                    colorfulC++;
                }
                if(mid){
                    if(norm_lv==2){
                        norm_lv=3;
                    }
                    bombC++;
                }
                if(pro){
                    if(norm_lv==3){
                        norm_lv=4;
                    }
                    laserC++;
                }
            }
            if(mode==2){
                if(intro){
                    if(inf_lv==1){
                        inf_lv=2;
                    }
                    colorfulC++;
                }
                if(mid){
                    if(inf_lv==2){
                        inf_lv=3;
                    }
                    bombC++;
                }
                if(pro){
                    if(inf_lv==3){
                        inf_lv=4;
                    }
                    laserC++;
                }
            }
            if(mode==3){
                if(intro){
                    if(time_lv==1){
                        time_lv=2;
                    }
                    colorfulC++;
                }
                if(mid){
                    if(time_lv==2){
                        time_lv=3;
                    }
                    bombC++;
                }
                if(pro){
                    if(time_lv==3){
                        time_lv=4;
                    }
                    laserC++;
                }
            }
            if(mode==4){
                if(intro){
                    if(hardcore_lv==1){
                        time_lv=2;
                    }
                    colorfulC++;
                }
                if(mid){
                    if(hardcore_lv==2){
                        time_lv=3;
                    }
                    bombC++;
                }
                if(pro){
                    if(hardcore_lv==3){
                        time_lv=4;
                    }
                    laserC++;
                }
            }
        }
        SDL_Rect score = {width/2-600/2, 360, 600, 60};
        SDL_Rect balls_popped = {width/2-600/2, 564, 600, 60};
        string s = "Total kill: ";
        if(mode==1){
            if(score_counter<temp1){
                score_counter+=5;
            }
            else{
                SDL_RenderCopy(renderer, win_sign, NULL, &result_sign_rect);
                stop = true;
            }

            if(score_counter<100){
                score.x = width/2-600/2;
            }
            else{
                score.x = width/2-600/2-15;
            }
            text(renderer, score.x+280, score.y+25, to_string(score_counter).c_str(), 238, 230, 198, 255, 2);
            s+=to_string(balls1);
        }
        if(mode==2){
            if(score_counter<temp2){
                score_counter+=5;
            }            else{
                SDL_RenderCopy(renderer, win_sign, NULL, &result_sign_rect);
                stop = true;
            }

            if(score_counter<100){
                score.x = width/2-600/2;
            }
            else{
                score.x = width/2-600/2-15;
            }
            text(renderer, score.x+280, score.y+25, to_string(score_counter).c_str(), 238, 230, 198, 255, 2);
            s+=to_string(balls2);
        }
        if(mode==3){
            if(score_counter<temp3){
                score_counter+=5;
            }
            else{
                SDL_RenderCopy(renderer, win_sign, NULL, &result_sign_rect);
                stop = true;
            }

            if(score_counter<100){
                score.x = width/2-600/2;
            }
            else{
                score.x = width/2-600/2-15;
            }
            text(renderer, score.x+280, score.y+25, to_string(score_counter).c_str(), 238, 230, 198, 255, 2);
            s+=to_string(balls3);
        }
        if(mode==4){
            if(score_counter<temp4){
                score_counter+=5;
            }
            else{
                SDL_RenderCopy(renderer, win_sign, NULL, &result_sign_rect);
                stop = true;
            }

            if(score_counter<100){
                score.x = width/2-600/2;
            }
            else{
                score.x = width/2-600/2-15;
            }
            text(renderer, score.x+280, score.y+25, to_string(score_counter).c_str(), 238, 230, 198, 255, 2);
            s+=to_string(balls4);
        }
        if(!stop){
            text(renderer, balls_popped.x+215, balls_popped.y+25, s.c_str(), 82, 72, 72, 255, 1);
        }
    }
    else{
        if(once){
            Mix_PlayMusic( game_lose_music, 0);
            once = false;
        }
        SDL_RenderCopy(renderer, lose_sign, NULL, &result_sign_rect);
        text(renderer, width/2-600/2+290, 360+25, "0", 238, 230, 198, 255, 2);
    }

    end_focus();

    if(button(to_home_rect) && l_press && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
        Mix_HaltMusic();
        gEnd = false;
        temp1 = 0;
        temp2 = 0;
        temp3 = 0;
        temp4 = 0;
        balls1 = 0;
        balls2 = 0;
        balls3 = 0;
        balls4 = 0;
        gNormal = false;
        gInfinite = false;
        gTimer = false;
        gEnd = false;
        score_counter = 0;
        win = false;
        gHome = true;
        once = true;
        l_press = false;
        return;
    }
    if(button(to_menu_rect) && l_press && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
        Mix_HaltMusic();
        gEnd = false;
        temp1 = 0;
        temp2 = 0;
        temp3 = 0;
        temp4 = 0;
        balls1 = 0;
        balls2 = 0;
        balls3 = 0;
        balls4 = 0;
        gNormal = false;
        gInfinite = false;
        gTimer = false;
        gEnd = false;
        score_counter = 0;
        win = false;
        gMenu = true;
        once = true;
        l_press = false;
        return;
    }
    if(button(replay_rect) && l_press && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
        Mix_HaltMusic();
        gEnd = false;
        temp1 = 0;
        temp2 = 0;
        temp3 = 0;
        temp4 = 0;
        balls1 = 0;
        balls2 = 0;
        balls4 = 0;
        balls3 = 0;
        score_counter = 0;
        win = false;
        if(mode==1){
            gNormal = true;
        }
        if(mode==2){
            gInfinite = true;
        }
        if(mode==3){
            gTimer = true;
        }
        if(mode==4){
            gHardcore = true;
        }
        once = true;
        l_press = false;
        return;
    }

    elapsedTicks = SDL_GetTicks() - startTicks;
    while (elapsedTicks < desiredDelta) {
        elapsedTicks = SDL_GetTicks() - startTicks;
    }
    SDL_RenderPresent(renderer);
}

//random color creator
void color(char& color1, char& color2){
    int a;
    if(mid || pro){a = 6;}else{a = 5;}
    switch(rand()%a){
        case 0:
            color1='r';
            color2='r';
            break;

        case 1:
            color1='g';
            color2='g';
            break;

        case 2:
            color1='b';
            color2='b';
            break;

        case 3:
            color1='y';
            color2='y';
            break;

        case 4:
            color1='p';
            color2='p';
            break;
        case 5:
            color1='o';
            color2='o';
            break;
    }
}

//random pattern creator
bool pattern(vector<vector<target_ball>>& vec, int i, int j, int n, char color1, char color2){
    if(n==0) return true;

    if(vec[i][j].color1=='\0' && vec[i][j].color2=='\0'){
        vec[i][j].color1=color1;
        vec[i][j].color2=color2;
    }
    else{
        return false;
    }

    int a=rand()%6;
    if(i%2==0){
        switch(a){
            case 0:
                if(j-1>=0){
                    bool result=pattern(vec, i, j-1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 1:
                if(j+1<vec[0].size()){
                    bool result=pattern(vec, i, j+1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 2:
                if(i-1>=0 && j-1>=0){
                    bool result=pattern(vec, i-1, j-1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 3:
                if(i-1>=0){
                    bool result=pattern(vec, i-1, j, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 4:
                if(i+1<vec.size() && j-1>=0){
                    bool result=pattern(vec, i+1, j-1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 5:
                if(i+1<vec.size()){
                    bool result=pattern(vec, i+1, j, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            default:
                break;
        }
    }
    else{
        switch(a){
            case 0:
                if(j-1>=0){
                    bool result=pattern(vec, i, j-1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 1:
                if(j+1<vec[0].size()){
                    bool result=pattern(vec, i, j+1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 2:
                if(i-1>=0){
                    bool result=pattern(vec, i-1, j, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 3:
                if(i-1>=0 && j+1<vec[0].size()){
                    bool result=pattern(vec, i-1, j+1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 4:
                if(i+1<vec.size()){
                    bool result=pattern(vec, i+1, j, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            case 5:
                if(i+1<vec.size() && j+1<vec[0].size()){
                    bool result=pattern(vec, i+1, j+1, n-1, color1, color2);
                    if(result){
                        break;
                    }
                }
            default:
                break;
        }
    }
    return true;
}

bool recolor(vector<vector<target_ball>>& vec, int i, int j, int n){
    if(n==0) return true;

    int a;
    if(mid || pro){a=1;}else{a=0;}

    if(vec[i][j].color1 == vec[i][j].color2){
        if(rand()%2==0){
            if(vec[i][j].color1=='b'){
                switch(rand()%4+a) {
                    case 0:
                        vec[i][j].color2='g';
                        break;
                    case 1:
                        vec[i][j].color2='r';
                        break;
                    case 2:
                        vec[i][j].color2='y';
                        break;
                    case 3:
                        vec[i][j].color2='p';
                        break;
                    case 4:
                        vec[i][j].color2='o';
                        break;
                }
            }

            if(vec[i][j].color1=='g'){
                switch(rand()%3+a) {
                    case 0:
                        vec[i][j].color2='r';
                        break;
                    case 1:
                        vec[i][j].color2='y';
                        break;
                    case 2:
                        vec[i][j].color2='p';
                        break;
                    case 3:
                        vec[i][j].color2='o';
                        break;
                }
            }

            if(vec[i][j].color1=='r'){
                switch(rand()%2+a) {
                    case 0:
                        vec[i][j].color2='y';
                        break;
                    case 1:
                        vec[i][j].color2='p';
                        break;
                    case 2:
                        vec[i][j].color2='o';
                        break;
                }
            }

            if(vec[i][j].color1=='y'){
                switch(rand()%1+a) {
                    case 0:
                        vec[i][j].color2='p';
                        break;
                    case 1:
                        vec[i][j].color2='o';
                        break;
                }
            }
            if(vec[i][j].color1=='p'){
                vec[i][j].color2='o';
            }
        }
        else{
            if(vec[i][j].color2=='b'){
                switch(rand()%4+a) {
                    case 0:
                        vec[i][j].color1='g';
                        break;
                    case 1:
                        vec[i][j].color1='r';
                        break;
                    case 2:
                        vec[i][j].color1='y';
                        break;
                    case 3:
                        vec[i][j].color1='p';
                        break;
                    case 4:
                        vec[i][j].color1='o';
                        break;
                }
            }

            if(vec[i][j].color2=='g'){
                switch(rand()%3+a) {
                    case 0:
                        vec[i][j].color1='r';
                        break;
                    case 1:
                        vec[i][j].color1='y';
                        break;
                    case 2:
                        vec[i][j].color1='p';
                        break;
                    case 3:
                        vec[i][j].color1='o';
                        break;
                }
            }

            if(vec[i][j].color2=='r'){
                switch(rand()%2+a) {
                    case 0:
                        vec[i][j].color1='y';
                        break;
                    case 1:
                        vec[i][j].color1='p';
                        break;
                    case 2:
                        vec[i][j].color1='o';
                        break;
                }
            }

            if(vec[i][j].color2=='y'){
                switch(rand()%1+a) {
                    case 0:
                        vec[i][j].color1='p';
                        break;
                    case 1:
                        vec[i][j].color1='o';
                        break;
                }
            }
            if(vec[i][j].color2=='p'){
                vec[i][j].color1='o';
            }
        }
    }
    else{
        return false;
    }

    int b=rand()%6;
    if(i%2==0){
        switch(b){
            case 0:
                if(j-1>=0){
                    bool result=recolor(vec, i, j-1, n-1);
                    if(result){
                        break;
                    }
                }
            case 1:
                if(j+1<vec[0].size()){
                    bool result=recolor(vec, i, j+1, n-1);
                    if(result){
                        break;
                    }
                }
            case 2:
                if(i-1>=0 && j-1>=0){
                    bool result=recolor(vec, i-1, j-1, n-1);
                    if(result){
                        break;
                    }
                }
            case 3:
                if(i-1>=0){
                    bool result=recolor(vec, i-1, j, n-1);
                    if(result){
                        break;
                    }
                }
            case 4:
                if(i+1<vec.size() && j-1>=0){
                    bool result=recolor(vec, i+1, j-1, n-1);
                    if(result){
                        break;
                    }
                }
            case 5:
                if(i+1<vec.size()){
                    bool result=recolor(vec, i+1, j, n-1);
                    if(result){
                        break;
                    }
                }
            default:
                break;
        }
    }
    else{
        switch(b){
            case 0:
                if(j-1>=0){
                    bool result=recolor(vec, i, j-1, n-1);
                    if(result){
                        break;
                    }
                }
            case 1:
                if(j+1<vec[0].size()){
                    bool result=recolor(vec, i, j+1, n-1);
                    if(result){
                        break;
                    }
                }
            case 2:
                if(i-1>=0){
                    bool result=recolor(vec, i-1, j, n-1);
                    if(result){
                        break;
                    }
                }
            case 3:
                if(i-1>=0 && j+1<vec[0].size()){
                    bool result=recolor(vec, i-1, j+1, n-1);
                    if(result){
                        break;
                    }
                }
            case 4:
                if(i+1<vec.size()){
                    bool result=recolor(vec, i+1, j, n-1);
                    if(result){
                        break;
                    }
                }
            case 5:
                if(i+1<vec.size() && j+1<vec[0].size()){
                    bool result=recolor(vec, i+1, j+1, n-1);
                    if(result){
                        break;
                    }
                }
            default:
                break;
        }
    }
    return true;
}

//checks for collision
bool isCollision(const target_ball &ball1, const target_ball &ball2){
    double d = square(ball1.x, ball1.y, ball2.x, ball2.y);
    return d - 200 < (2*ball1.rad)*(2*ball1.rad);
}

bool findPath1(vector<vector<target_ball>>& vec, int row, int col, int m, int n){
    vec[row][col].traverse = true;

    if (row == m && col == n) {
        return true;
    }

    for (int i = std::max(0, row - 1); i <= std::min(row + 1, static_cast<int>(vec.size() - 1)); ++i) {
        for (int j = 0; j < vec[i].size(); ++j) {
            if (isValidMove(i, j) &&
                !vec[i][j].traverse && !vec[i][j].stone && vec[i][j].color1 == vec[m][n].color1 && vec[i][j].color2 == vec[m][n].color2 &&
                isCollision(vec[row][col], vec[i][j])) {
                if (findPath1(vec, i, j, m, n)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void prison(vector<vector<target_ball>>& vec, int m, int n){
    for (int i = 0; i < tg_vector.size(); i++) {
        for (int j = 0; j < tg_vector[i].size(); j++) {
            if(vec[i][j].color1 == vec[m][n].color1 && vec[i][j].color2 == vec[m][n].color2 && !vec[i][j].stone) {
                if (findPath1(tg_vector, i, j, m, n)) {
                    vec[i][j].chain = true;
                }
            }
            for (auto &row : vec) {
                for (auto &ball : row) {
                    ball.traverse = false;
                }
            }
        }
    }
    vec[m][n].chain = true;
}

//returns sum of square
double square(double a, double b, double c, double d){
    return (a-c)*(a-c)+(b-d)*(b-d);
}

void game_draw(SDL_Renderer* renderer, bool y){
    wall_rect={-28,0,252,750};
    SDL_RenderCopy(renderer, wall_texture, NULL, &wall_rect);
    wall_rect.x=width-252+28;
    SDL_RenderCopy(renderer, wall_texture, NULL, &wall_rect);
    SDL_RenderCopy(renderer, bullet, NULL, &bullet_rect);
    SDL_RenderCopy(renderer, texture_all, NULL, &magic_rect1);
    string a1 = "*";
    a1+=to_string(colorfulC);
    text(renderer, magic_rect1.x+58, magic_rect1.y+2, a1.c_str(), 255, 000, 000, 255, 2);
    SDL_RenderCopy(renderer, texture_bomb, NULL, &magic_rect2);
    string a2 = "*";
    a2+=to_string(bombC);
    text(renderer, magic_rect2.x+58, magic_rect2.y+2, a2.c_str(), 000, 255, 000, 255, 2);
    SDL_RenderCopy(renderer, texture_laser, NULL, &magic_rect3);
    string a3 = "*";
    a3+=to_string(laserC);
    text(renderer, magic_rect3.x+58, magic_rect3.y+2, a3.c_str(), 000, 000, 255, 255, 2);
    SDL_RenderCopy(renderer, texture_thunder, NULL, &magic_rect4);
    string a4 = "*";
    a4+=to_string(thunderC);
    text(renderer, magic_rect4.x+58, magic_rect4.y+2, a4.c_str(), 125, 125, 125, 255, 2);

    if(y){
        if(statX==0 && statY ==0){
            statX=m_x;
            statY=m_y;
        }
    }
    else{
        statX=m_x;
        statY=m_y;
    }

    float centerX = width/2;
    float centerY = height-14;
    double dx = statX - centerX;
    double dy = statY - centerY;
    double angle = atan2(dy, dx) * 180 / M_PI;
    center = {cannon_rect.w/2, cannon_rect.h};
    angle += 90;
    SDL_RenderCopyEx(renderer, cannon, NULL, &cannon_rect, angle, &center, SDL_FLIP_NONE);
}

void ball_drawer(SDL_Renderer* renderer, char& ball1, char& ball2, bool all, bool stone, bool bomb, bool prisoner, bool slow, bool stop, bool laser, bool thunder){
    if(all){
        SDL_RenderCopyF(renderer, texture_all, nullptr, &BallRect);
        return;
    }
    if(stone){
        SDL_RenderCopyF(renderer, texture_stone, nullptr, &BallRect);
        return;
    }
    if(bomb){
        SDL_RenderCopyF(renderer, texture_bomb, nullptr, &BallRect);
        return;
    }
    if(laser){
        SDL_RenderCopyF(renderer, texture_laser, nullptr, &BallRect);
        return;
    }
    if(thunder){
        SDL_RenderCopyF(renderer, texture_thunder, nullptr, &BallRect);
        return;
    }

    if (ball1 == 'r' && ball2 == 'r') {
        SDL_RenderCopyF(renderer, texture_r, nullptr, &BallRect);
    }
    if (ball1 == 'g' && ball2 == 'g') {
        SDL_RenderCopyF(renderer, texture_g, nullptr, &BallRect);
    }
    if (ball1 == 'b' && ball2 == 'b') {
        SDL_RenderCopyF(renderer, texture_b, nullptr, &BallRect);
    }
    if (ball1 == 'p' && ball2 == 'p') {
        SDL_RenderCopyF(renderer, texture_p, nullptr, &BallRect);
    }
    if (ball1 == 'y' && ball2 == 'y') {
        SDL_RenderCopyF(renderer, texture_y, nullptr, &BallRect);
    }
    if (ball1 == 'o' && ball2 == 'o') {
        SDL_RenderCopyF(renderer, texture_o, nullptr, &BallRect);
    }

    if ((ball1 == 'b' && ball2 == 'g') || (ball1 == 'g' && ball2 == 'b')) {
        SDL_RenderCopyF(renderer, texture_bg, nullptr, &BallRect);
    }
    if ((ball1 == 'b' && ball2 == 'r') || (ball1 == 'r' && ball2 == 'b')) {
        SDL_RenderCopyF(renderer, texture_br, nullptr, &BallRect);
    }
    if ((ball1 == 'b' && ball2 == 'y') || (ball1 == 'y' && ball2 == 'b')) {
        SDL_RenderCopyF(renderer, texture_by, nullptr, &BallRect);
    }
    if ((ball1 == 'b' && ball2 == 'p') || (ball1 == 'p' && ball2 == 'b')) {
        SDL_RenderCopyF(renderer, texture_bp, nullptr, &BallRect);
    }
    if ((ball1 == 'b' && ball2 == 'o') || (ball1 == 'o' && ball2 == 'b')) {
        SDL_RenderCopyF(renderer, texture_bo, nullptr, &BallRect);
    }

    if ((ball1 == 'g' && ball2 == 'r') || (ball1 == 'r' && ball2 == 'g')) {
        SDL_RenderCopyF(renderer, texture_gr, nullptr, &BallRect);
    }
    if ((ball1 == 'g' && ball2 == 'y') || (ball1 == 'y' && ball2 == 'g')) {
        SDL_RenderCopyF(renderer, texture_gy, nullptr, &BallRect);
    }
    if ((ball1 == 'g' && ball2 == 'p') || (ball1 == 'p' && ball2 == 'g')) {
        SDL_RenderCopyF(renderer, texture_gp, nullptr, &BallRect);
    }
    if ((ball1 == 'g' && ball2 == 'o') || (ball1 == 'o' && ball2 == 'g')) {
        SDL_RenderCopyF(renderer, texture_go, nullptr, &BallRect);
    }

    if ((ball1 == 'r' && ball2 == 'y') || (ball1 == 'y' && ball2 == 'r')) {
        SDL_RenderCopyF(renderer, texture_ry, nullptr, &BallRect);
    }
    if ((ball1 == 'r' && ball2 == 'p') || (ball1 == 'p' && ball2 == 'r')) {
        SDL_RenderCopyF(renderer, texture_rp, nullptr, &BallRect);
    }
    if ((ball1 == 'r' && ball2 == 'o') || (ball1 == 'o' && ball2 == 'r')) {
        SDL_RenderCopyF(renderer, texture_ro, nullptr, &BallRect);
    }

    if ((ball1 == 'y' && ball2 == 'p') || (ball1 == 'p' && ball2 == 'y')) {
        SDL_RenderCopyF(renderer, texture_yp, nullptr, &BallRect);
    }
    if ((ball1 == 'y' && ball2 == 'o') || (ball1 == 'o' && ball2 == 'y')) {
        SDL_RenderCopyF(renderer, texture_yo, nullptr, &BallRect);
    }

    if ((ball1 == 'p' && ball2 == 'o') || (ball1 == 'o' && ball2 == 'p')) {
        SDL_RenderCopyF(renderer, texture_po, nullptr, &BallRect);
    }

    if(prisoner){
        SDL_RenderCopyF(renderer, chain, nullptr, &BallRect);
        return;
    }
    if(slow){
        SDL_RenderCopyF(renderer, texture_slow, nullptr, &BallRect);
        return;
    }
    if(stop){
        SDL_RenderCopyF(renderer, texture_stop, nullptr, &BallRect);
        return;
    }
}

void reInit(){
    bomb_frame=-1;
    laser_frame=-1;
    tg_ball.color1='\0';
    tg_ball.color2='\0';
    tg_ball.x=0, tg_ball.y=0;
    tg_ball.visit=false;
    tg_ball.traverse=false;
    tg_ball.show=false;
    tg_ball.falling=false;
    tg_ball.all=false;
    tg_ball.stone=false;
    tg_ball.chain=false;
    tg_ball.slow=false;
    tg_ball.stop=false;

    sh_ball.color1='\0';
    sh_ball.color2='\0';
    sh_ball.x=width/2;

    wait = -1600;
    x_speed = 0;
    y_speed = 0;

    inf_count = 0;
    startTimer = 0;

    tg_vector.resize(0);
    sh_vector.resize(0);
    game_finish = false;
    once = true;
}

//checks for deletion candidates
void remover(vector<vector<target_ball>>& vec1, vector<shoot_ball>& vec2, int i, int j, int& count){
    if(vec1[i][j].show && !vec1[i][j].visit && !vec1[i][j].stone && (vec2[0].color1 == vec1[i][j].color1 || vec2[0].color1 == vec1[i][j].color2 || vec1[i][j].all)){
        count++;
        vec1[i][j].visit=true;
    }
    else{
        return;
    }

    for (int a = max(0, i - 1); a <= min(i + 1, static_cast<int>(vec1.size() - 1)); a++) {
        for (int b = 0; b < vec1[a].size(); b++) {
            double d = square(vec1[a][b].x, vec1[a][b].y, vec1[i][j].x, vec1[i][j].y);
            if (d - 200 <= (2 * vec2[0].rad) * (2 * vec2[0].rad)) {
                remover(vec1, vec2, a, b, count);
            }
        }
    }
}

//removes candidates
void terminator(vector<vector<target_ball>>& vec1){
    for(int a=0; a<vec1.size(); a++){
        for(int b=0; b<vec1[a].size(); b++){
            if(vec1[a][b].visit){
                if(vec1[a][b].chain){
                    vec1[a][b].chain = false;
                    vec1[a][b].visit = false;
                }
                else{
                    vec1[a][b].show=false;
                }
            }
        }
    }
}

//checks if i and j values exists
bool isValidMove(int row, int col){
    return row >= 0 && row < tg_vector.size() && col >= 0 && col < tg_vector[row].size();
}

//checks if ball should fall or not
bool findPath(vector<vector<target_ball>>& vec, int row, int col){
    vec[row][col].traverse = true;

    if (row == vec.size() - 1) {
        return true;
    }

    for (int i = std::max(0, row - 1); i <= std::min(row + 1, static_cast<int>(vec.size() - 1)); ++i) {
        for (int j = 0; j < vec[i].size(); ++j) {
            if (isValidMove(i, j) &&
                !vec[i][j].traverse && vec[i][j].show &&
                isCollision(vec[row][col], vec[i][j])) {
                if(vec[i][j].visit){
                    if(vec[i][j].chain){
                        if (findPath(vec, i, j)) {
                            return true;
                        }
                    }
                }
                else{
                    if (findPath(vec, i, j)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

//feeds the findPath all balls
void climber(vector<vector<target_ball>>& vec){
    for (int i = 0; i < tg_vector.size(); ++i) {
        for (int j = 0; j < tg_vector[i].size(); ++j) {
            if (!findPath(tg_vector, i, j)) {
                vec[i][j].falling = true;
            }
            for (auto &row : vec) {
                for (auto &ball : row) {
                    ball.traverse = false;
                }
            }
        }
    }
}

void explosion(SDL_Renderer* renderer, int n){
    int x, y;
    if(n/2==8){
        SDL_QueryTexture(frame1, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame1, NULL, &exp_rect);
    }
    if(n/2==7){
        SDL_QueryTexture(frame2, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame2, NULL, &exp_rect);
    }
    if(n/2==6){
        SDL_QueryTexture(frame3, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame3, NULL, &exp_rect);
    }
    if(n/2==5){
        SDL_QueryTexture(frame4, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame4, NULL, &exp_rect);
    }
    if(n/2==4){
        SDL_QueryTexture(frame5, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame5, NULL, &exp_rect);
    }
    if(n/2==3){
        SDL_QueryTexture(frame6, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame6, NULL, &exp_rect);
    }
    if(n/2==2){
        SDL_QueryTexture(frame7, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame7, NULL, &exp_rect);
    }
    if(n/2==1){
        SDL_QueryTexture(frame8, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame8, NULL, &exp_rect);
    }
    if(n/2==0){
        SDL_QueryTexture(frame9, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, frame9, NULL, &exp_rect);
    }
}

void bomb(SDL_Renderer* renderer, int n){
    int x, y;
    exp_rect.x = bombX-24-150;
    exp_rect.y = bombY-24-150;
    if(n/2==8){
        SDL_QueryTexture(fr1, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr1, NULL, &exp_rect);
    }
    if(n/2==7){
        SDL_QueryTexture(fr2, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr2, NULL, &exp_rect);
    }
    if(n/2==6){
        SDL_QueryTexture(fr3, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr3, NULL, &exp_rect);
    }
    if(n/2==5){
        SDL_QueryTexture(fr4, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr4, NULL, &exp_rect);
    }
    if(n/2==4){
        SDL_QueryTexture(fr5, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr5, NULL, &exp_rect);
    }
    if(n/2==3){
        SDL_QueryTexture(fr6, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr6, NULL, &exp_rect);
    }
    if(n/2==2){
        SDL_QueryTexture(fr7, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr7, NULL, &exp_rect);
    }
    if(n/2==1){
        SDL_QueryTexture(fr8, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr8, NULL, &exp_rect);
    }
    if(n/2==0){
        SDL_QueryTexture(fr9, nullptr, nullptr, &x, &y);
        exp_rect.w=x;
        exp_rect.h=y;
        SDL_RenderCopy(renderer, fr9, NULL, &exp_rect);
    }
}

void laser_anime(SDL_Renderer* renderer, int n){
    float centerX = width/2;
    float centerY = height-14;
    double dx = m_x - centerX;
    double dy = m_y - centerY;
    double angle = atan2(dy, dx) * 180 / M_PI;
    center = {laser_rect.w/2, laser_rect.h};
    angle += 90;
    if(n/5==3){
        SDL_RenderCopyEx(renderer, fram1, NULL, &laser_rect, angle, &center, SDL_FLIP_NONE);
    }
    if(n/5==2){
        SDL_RenderCopyEx(renderer, fram2, NULL, &laser_rect, angle, &center, SDL_FLIP_NONE);
    }
    if(n/5==1){
        SDL_RenderCopyEx(renderer, fram3, NULL, &laser_rect, angle, &center, SDL_FLIP_NONE);
    }
    if(n/5==0){
        SDL_RenderCopyEx(renderer, fram4, NULL, &laser_rect, angle, &center, SDL_FLIP_NONE);
    }
}

//normal mode tg_vec initializer
void normal_init(){
    for (int i = 0; i < 15; i++) {
        tg_vector.emplace_back(15);
        for (int j = 0; j < 15; j++) {
            if (i % 2 == 0) {
                tg_vector.back()[j] = {24, 17, '\0', '\0',
                                       252.0 + j * 48, 3 * 24.0 - i * 41,
                                       false, false, true, false, false, false, false, false, false};
            } else {
                tg_vector.back()[j] = {24, 17, '\0', '\0',
                                       276.0 + j * 48, 3 * 24.0 - i * 41,
                                       false, false, true, false, false, false, false, false, false};
            }
        }
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (tg_vector[i][j].color1 == '\0' && tg_vector[i][j].color2 == '\0') {
                int a = rand() % 5 + 3;
                color(tg_ball.color1, tg_ball.color2);
                pattern(tg_vector, i, j, a, tg_ball.color1, tg_ball.color2);
            }
        }
    }

    int c = rand()%15;
    int d = rand()%15;
    prison(tg_vector, c, d);

    for(int i=0; i<7; i++){
        int n = rand()%5+5;
        int a = rand()%15;
        int b = rand()%15;
        recolor(tg_vector, a, b, n);
    }

    int a=rand()%14;
    int b=rand()%15;
    tg_vector[a][b].stone=true;

    //sh_vector initialization
    color(sh_ball.color1, sh_ball.color2);
    sh_vector.push_back(sh_ball);

    color(sh_ball.color1, sh_ball.color2);
    sh_ball.x = width/2-96;
    sh_vector.push_back(sh_ball);
}

//tg_ball drawer and collision detector
void tg_ball_draw(SDL_Renderer* renderer, bool x, bool y){
    for (int i = 0; i < tg_vector.size(); i++) {
        auto &row = tg_vector[i];

        for (int j = 0; j < tg_vector[i].size(); j++) {
            auto &ball = tg_vector[i][j];

            if(!ball.show && ball.n >= 0){
                --ball.n;
                exp_rect.x = ball.x - ball.rad;
                exp_rect.y = ball.y - ball.rad;
                explosion(renderer, ball.n);
            }

            if (ball.y >= -24 && ball.show) {
                BallRect.x = ball.x - ball.rad;
                BallRect.y = ball.y - ball.rad;
                if(rand()%10000000==3 && !y){
                    if(!ball.slow){
                        ball.slow = true;
                        time_effect = SDL_GetTicks();
                    }
                }
                if(rand()%10000000==8 && !y){
                    if(!ball.stop){
                        ball.stop = true;
                        time_effect = SDL_GetTicks();
                    }
                }

                if(ball.slow && SDL_GetTicks() - time_effect > 5000 && (ball.show && !ball.falling) && !slow_motion){
                    ball.slow = false;
                }
                if(ball.stop && SDL_GetTicks() - time_effect > 5000 && (ball.show && !ball.falling) && !stop_motion){
                    ball.stop = false;
                }
                ball_drawer(renderer, ball.color1, ball.color2, ball.all, ball.stone, false, ball.chain, ball.slow, ball.stop, false, false);
            }
            if(!y){
                if (ball.falling && ball.show) {
                    ball.y += 10;
                } else if(!x && !stop_motion && !slow_motion && !ball.falling && ball.show){
                    ball.y += 0.5;
                } else if(stop_motion && !x && !ball.falling && ball.show){
                    ball.y += 0;
                } else if(slow_motion && !x && !ball.falling && ball.show){
                    ball.y += 0.2;
                }
            }
        }
    }
    if(bomb_frame>=0){
        --bomb_frame;
        bomb(renderer, bomb_frame);
    }
    if(laser_frame>=0){
        --laser_frame;
        laser_anime(renderer, laser_frame);
    }
}

void ball_stat(){
    for (int i = 0; i < tg_vector.size(); i++) {
        auto &row = tg_vector[i];
        for (int j = 0; j < tg_vector[i].size(); j++) {
            auto &ball = tg_vector[i][j];

            if(ball.slow && (ball.falling || !ball.show)){
                ball.slow = false;
                effect_counter=SDL_GetTicks();
                slow_motion = true;
            }
            if(ball.stop && (ball.falling || !ball.show)){
                ball.stop = false;
                effect_counter=SDL_GetTicks();
                stop_motion = true;
            }
            if(SDL_GetTicks() - effect_counter > 5000){
                slow_motion = false;
                stop_motion = false;
            }

            if (ball.show && !ball.falling) {
                for(int k = 0; k <sh_vector.size(); k++) {
                    double a = square(ball.x, ball.y, sh_vector[k].x, sh_vector[k].y);
                    if (a - 200 <= (2 * ball.rad) * (2 * ball.rad)) {
                        if (ball.color1 == sh_vector[k].color1 || ball.color2 == sh_vector[k].color1 || sh_vector[0].all || sh_vector[0].bomb || ball.all) {
                            col_ball.i = i;
                            col_ball.j = j;
                            col_ball.color = true;
                        }
                        else{
                            col_ball.i = i;
                            col_ball.j = j;
                            col_ball.color = false;
                        }
                        collide.push_back(col_ball);
                    }
                }
            }

            if(ball.falling && ball.y > height) {
                ball.show = false;
            }
            if(ball.y>600-12 && ball.show && !ball.falling){
                game_finish = true;
                win = false;
            }
        }
    }
}

//sh_vec collision function
bool collision(SDL_Renderer* renderer){
    bool x = false;
    if (!collide.empty()) {
        int count = 0;
        bool begin = false;

        if (collide[0].i - 1 >= 0) {
            if(sh_vector[0].all){
                sh_vector[0].color1 = tg_vector[collide[0].i][collide[0].j].color1;
                sh_vector[0].color2 = tg_vector[collide[0].i][collide[0].j].color2;
            }
            for (int k = 0; k < tg_vector[collide[0].i - 1].size(); k++) {
                if (tg_vector[collide[0].i - 1][k].x < sh_vector[0].x && tg_vector[collide[0].i - 1][k].show) {
                    count++;
                }
            }

            if(sh_vector[0].all){
                tg_vector[collide[0].i - 1].insert(tg_vector[collide[0].i - 1].begin() + count, {
                        sh_vector[0].rad, 17,
                        tg_vector[collide[0].i][collide[0].j].color1, tg_vector[collide[0].i][collide[0].j].color2,
                        sh_vector[0].x,
                        sh_vector[0].y, tg_ball.visit, tg_ball.traverse, true, false, sh_vector[0].all, false, false, false, false});
            }
            else{
                tg_vector[collide[0].i - 1].insert(tg_vector[collide[0].i - 1].begin() + count, {
                        sh_vector[0].rad, 17,
                        sh_vector[0].color1, sh_vector[0].color2,
                        sh_vector[0].x,
                        sh_vector[0].y, tg_ball.visit, tg_ball.traverse, true, false, sh_vector[0].all, false, false, false, false});
                if(tg_vector[collide[0].i][collide[0].j].chain && (tg_vector[collide[0].i][collide[0].j].color1 == sh_vector[0].color1 ||
                tg_vector[collide[0].i][collide[0].j].color1 == sh_vector[0].color2) &&
                (tg_vector[collide[0].i][collide[0].j].color2 == sh_vector[0].color1 ||
                tg_vector[collide[0].i][collide[0].j].color2 == sh_vector[0].color2)){
                    tg_vector[collide[0].i - 1][count].chain = true;
                }
            }

            while (true) {
                double d = square(tg_vector[collide[0].i][collide[0].j].x,
                                  tg_vector[collide[0].i][collide[0].j].y,
                                  tg_vector[collide[0].i - 1][count].x,
                                  tg_vector[collide[0].i - 1][count].y);

                if (d < (2 * sh_vector[0].rad) * (2 * sh_vector[0].rad)) {
                    if (tg_vector[collide[0].i][collide[0].j].x < tg_vector[collide[0].i - 1][count].x) {
                        ++tg_vector[collide[0].i - 1][count].x;
                    } else if (tg_vector[collide[0].i][collide[0].j].x > tg_vector[collide[0].i - 1][count].x) {
                        --tg_vector[collide[0].i - 1][count].x;
                    }

                    if (tg_vector[collide[0].i][collide[0].j].y < tg_vector[collide[0].i - 1][count].y) {
                        ++tg_vector[collide[0].i - 1][count].y;
                    } else if (tg_vector[collide[0].i][collide[0].j].y > tg_vector[collide[0].i - 1][count].y) {
                        --tg_vector[collide[0].i - 1][count].y;
                    }
                } else {
                    break;
                }
            }
        } else {
            if(sh_vector[0].all){
                sh_vector[0].color1 = tg_vector[collide[0].i+1][collide[0].j].color1;
                sh_vector[0].color2 = tg_vector[collide[0].i+1][collide[0].j].color2;
            }
            begin = true;
            vector<target_ball> temp;
            temp.resize(15);

            int whatever = false;

            if(sh_vector[0].all){
                temp[count] = {
                        sh_vector[0].rad, 17,
                        tg_vector[collide[0].i+1][collide[0].j].color1, tg_vector[collide[0].i+1][collide[0].j].color2,
                        sh_vector[0].x,
                        sh_vector[0].y, tg_ball.visit, tg_ball.traverse, true, false, sh_vector[0].all, false, false, false, false};
            }
            else{
                temp[count] = {
                        sh_vector[0].rad, 17,
                        sh_vector[0].color1, sh_vector[0].color2,
                        sh_vector[0].x,
                        sh_vector[0].y, tg_ball.visit, tg_ball.traverse, true, false, sh_vector[0].all, false, false, false, false};

                if(tg_vector[1][collide[0].j].chain && (tg_vector[1][collide[0].j].color1 == sh_vector[0].color1 ||
                    tg_vector[1][collide[0].j].color1 == sh_vector[0].color2) &&
                   (tg_vector[1][collide[0].j].color2 == sh_vector[0].color1 ||
                    tg_vector[1][collide[0].j].color2 == sh_vector[0].color2)){
                    whatever = true;
                }
            }
            tg_vector.insert(tg_vector.begin(), temp);
            if(whatever){
                tg_vector[0][count].chain = true;
            }

            while (true) {
                int d = square(tg_vector[1][collide[0].j].x,
                               tg_vector[1][collide[0].j].y,
                               tg_vector[0][count].x,
                               tg_vector[0][count].y);

                if (d < (2 * sh_vector[0].rad) * (2 * sh_vector[0].rad)) {
                    if (tg_vector[1][collide[0].j].x < tg_vector[0][count].x) {
                        ++tg_vector[0][count].x;
                    } else if (tg_vector[1][collide[0].j].x > tg_vector[0][count].x) {
                        --tg_vector[0][count].x;
                    }

                    if (tg_vector[1][collide[0].j].y < tg_vector[0][count].y) {
                        ++tg_vector[0][count].y;
                    } else if (tg_vector[1][collide[0].j].y > tg_vector[0][count].y) {
                        --tg_vector[0][count].y;
                    }
                }else{
                    break;
                }
            }
        }

        bool has_color = false;

        for (int i = 0; i < collide.size(); i++) {
            if(sh_vector[0].bomb){
                Mix_PlayChannel(-1, bomb_explosion_chunk, 0);
                x = true;
                bombX = sh_vector[0].x;
                bombY = sh_vector[0].y;
                bomb_frame = 17;
                for(int k=0; k<tg_vector.size(); k++){
                    for(int h=0; h<tg_vector[k].size(); h++){
                        if(150*150>square(sh_vector[0].x, sh_vector[0].y, tg_vector[k][h].x,tg_vector[k][h].y)){
                            tg_vector[k][h].show = false;
                        }
                    }
                }
                climber(tg_vector);
                terminator(tg_vector);
                sh_vector[0] = sh_vector[1];
                sh_vector.pop_back();
                collide.resize(0);
                break;
            }

            if (collide[i].color) {
                int count_b = 0;

                if (begin) {
                    remover(tg_vector, sh_vector, 1, collide[i].j, count_b);
                    if(count_b <= 2 && tg_vector[1][collide[i].j].chain){
                        terminator(tg_vector);
                    }
                } else {
                    remover(tg_vector, sh_vector, collide[i].i, collide[i].j, count_b);
                    if(count_b <= 2 && tg_vector[collide[i].i][collide[i].j].chain){
                        terminator(tg_vector);
                    }
                }

                if (count_b > 2) {
                    Mix_PlayChannel(-1, ball_explosion_chunk, 0);
                    climber(tg_vector);
                    terminator(tg_vector);
                    has_color = true;
                    x = true;
                    break;
                } else {
                    for (int j = 0; j < tg_vector.size(); j++) {
                        for (int k = 0; k < tg_vector[j].size(); k++) {
                            if (tg_vector[j][k].visit) {
                                tg_vector[j][k].visit = false;
                            }
                        }
                    }
                }
            }
        }

        if (!has_color) {
            if (collide[0].i - 1 >= 0) {
                BallRect.x = tg_vector[collide[0].i - 1][count].x - tg_vector[collide[0].i - 1][count].rad;
                BallRect.y = tg_vector[collide[0].i - 1][count].y - tg_vector[collide[0].i - 1][count].rad;
                ball_drawer(renderer, tg_vector[collide[0].i - 1][count].color1, tg_vector[collide[0].i - 1][count].color2, tg_vector[collide[0].i - 1][count].all, tg_vector[collide[0].i - 1][count].stone, false, false, false, false, false, false);
            }else{

                BallRect.x = tg_vector[0][count].x - tg_vector[0][count].rad;
                BallRect.y = tg_vector[0][count].y - tg_vector[0][count].rad;
                ball_drawer(renderer, tg_vector[0][count].color1, tg_vector[0][count].color2, tg_vector[0][count].all, tg_vector[0][count].stone, false, false, false, false, false, false);
            }
        }

        sh_vector[0] = sh_vector[1];
        sh_vector.pop_back();
        collide.resize(0);
    }

    return x;
}

//sh_vec filler
void sh_vec_filler(){
    if (sh_vector.size()!=2) {
        int count = 0;
        for(int i = 0; i < tg_vector.size(); i++) {
            for(int j = 0; j < tg_vector[i].size(); j++){
                if(tg_vector[i][j].show && !tg_vector[i][j].falling){
                    if(tg_vector[i][j].color1=='r' || tg_vector[i][j].color2=='r'){
                        holder.r='r';
                    }
                    if(tg_vector[i][j].color1=='g' || tg_vector[i][j].color2=='g'){
                        holder.g='g';
                    }
                    if(tg_vector[i][j].color1=='b' || tg_vector[i][j].color2=='b'){
                        holder.b='b';
                    }
                    if(tg_vector[i][j].color1=='p' || tg_vector[i][j].color2=='p'){
                        holder.p='p';
                    }
                    if(tg_vector[i][j].color1=='y' || tg_vector[i][j].color2=='y'){
                        holder.y='y';
                    }
                    if(tg_vector[i][j].color1=='o' || tg_vector[i][j].color2=='o'){
                        holder.o='o';
                    }
                    count++;
                }
            }
        }

        if(count == 0){
            game_finish = true;
            win = true;
            return;
        }

        color(sh_ball.color1, sh_ball.color2);
        while(sh_ball.color1!=holder.r && sh_ball.color1!=holder.g && sh_ball.color1!=holder.b && sh_ball.color1!=holder.p && sh_ball.color1!=holder.y && sh_ball.color1!=holder.o){
            color(sh_ball.color1, sh_ball.color2);
        }
        sh_vector.push_back(sh_ball);
        sh_vector[0].x=width/2;

        x_speed = 0;
        y_speed = 0;
        holder.r='k';
        holder.g='k';
        holder.b='k';
        holder.p='k';
        holder.y='k';
        holder.y='o';
    }
}

void sh_vec_draw(SDL_Renderer* renderer, bool x){
    auto &ball = sh_vector[0];

    if(x){
        if(statX==0 && statY ==0){
            statX=m_x;
            statY=m_y;
        }
    }
    else{
        statX=m_x;
        statY=m_y;
    }

    if(!ball.launch){
        double cosB = (statX - width / 2) / sqrt(square(statX, statY, width / 2, height - 14));
        double sinB = ((-1)*statY + height - 14) / sqrt(square(statX, statY, width / 2, height - 14));
        ball.x = cosB*39+width/2;
        ball.y = height-14-sinB*39;
    }

    BallRect.x = ball.x - ball.rad;
    BallRect.y = ball.y - ball.rad;

    if(laser_power){
        ball_drawer(renderer, ball.color1, ball.color2, false, false, false, false, false, false, true, false);
    }
    else{
        ball_drawer(renderer, ball.color1, ball.color2, ball.all, false, ball.bomb, false, false, false, false, false);
    }

    if(thunder_power){
        ball_drawer(renderer, ball.color1, ball.color2, false, false, false, false, false, false, false, true);
    }
    else if(!laser_power){
        ball_drawer(renderer, ball.color1, ball.color2, ball.all, false, ball.bomb, false, false, false, false, false);
    }

    if(l_press && !x && !ball.launch) {
        ball.launch = true;
        if(m_x != width/2){
            double cos = (m_x - width / 2) / sqrt(square(m_x, m_y, width / 2, height - 24));
            double sin = ((-1)*m_y + height - 24) / sqrt(square(m_x, m_y, width / 2, height - 24));
            x_speed = speed * cos;
            y_speed = (speed * sin)*(-1);
        }
        else{
            x_speed = 0;
            y_speed = (-1)*speed;
        }

        l_press = false;
    }

    if(!x){
        ball.x += x_speed;
        ball.y += y_speed;
    }

    if (ball.x >= width-252 || ball.x <= 252) {
        x_speed *= -1;
        while (ball.x >= 1000 || ball.x <= 200) {
            if (ball.x >= 1000) {
                --ball.x;
            } else {
                ++ball.x;
            }
        }
    }

    if(ball.y <= tg_vector[tg_vector.size()-1][0].y){
        if(ball.bomb){
            ball.bomb = false;
        }
        else{
            int count = 0;
            for (int k = 0; k < tg_vector[tg_vector.size()-1].size(); k++) {
                if (tg_vector[tg_vector.size()-1][k].x < ball.x && tg_vector[tg_vector.size()-1][k].show) {
                    count++;
                }
            }
            tg_vector[tg_vector.size()-1].insert(tg_vector[tg_vector.size()-1].begin() + count, {
                    ball.rad, 17,
                    ball.color1, ball.color2,
                    ball.x, tg_vector[tg_vector.size()-1][0].y,
                    false, false, true, false, ball.all, false,false, false, false});

            sh_vector[0] = sh_vector[1];
            sh_vector.pop_back();
        }
    }

    if(sh_vector.size()!=2) {
        for(int i = 0; i < tg_vector.size(); i++) {
            for(int j = 0; j < tg_vector[i].size(); j++){
                if(tg_vector[i][j].show && !tg_vector[i][j].falling){
                    if(tg_vector[i][j].color1=='r' || tg_vector[i][j].color2=='r'){
                        holder.r='r';
                    }
                    if(tg_vector[i][j].color1=='g' || tg_vector[i][j].color2=='g'){
                        holder.g='g';
                    }
                    if(tg_vector[i][j].color1=='b' || tg_vector[i][j].color2=='b'){
                        holder.b='b';
                    }
                    if(tg_vector[i][j].color1=='p' || tg_vector[i][j].color2=='p'){
                        holder.p='p';
                    }
                    if(tg_vector[i][j].color1=='y' || tg_vector[i][j].color2=='y'){
                        holder.y='y';
                    }
                    if(tg_vector[i][j].color1=='o' || tg_vector[i][j].color2=='o'){
                        holder.o='o';
                    }
                }
            }
        }

        sh_vector[0] = sh_vector[1];
        sh_vector[0].x=width/2;
        x_speed = 0;
        y_speed = 0;
        sh_vector.pop_back();

        color(sh_ball.color1, sh_ball.color2);
        while(sh_ball.color1!=holder.r && sh_ball.color1!=holder.g && sh_ball.color1!=holder.b && sh_ball.color1!=holder.p && sh_ball.color1!=holder.y){
            color(sh_ball.color1, sh_ball.color2);
        }
        sh_vector.push_back(sh_ball);

        holder.r='k';
        holder.g='k';
        holder.b='k';
        holder.p='k';
        holder.y='k';
    }

    BallRect.x=sh_vector[1].x-sh_vector[1].rad;
    BallRect.y=sh_vector[1].y-sh_vector[1].rad;
    ball_drawer(renderer, sh_vector[1].color1, sh_vector[1].color2, sh_vector[1].all, false, sh_vector[1].bomb, false, false, false, false, false);
}

void guide(SDL_Renderer* renderer, bool k){
    SDL_SetRenderDrawColor(renderer, 255, 000, 000, 255);
    double y=height-24, x=width/2, add=30;
    double sin, cos;
    bool get_out=false;

    if(k){
        if(statX==0 && statY ==0){
            statX=m_x;
            statY=m_y;
        }
    }
    else{
        statX=m_x;
        statY=m_y;
    }

    if(statX != width/2){
        sin = ((-1)*statY + height - 24)/sqrt(square(width/2, height - 24, statX, statY));
        cos = (statX - width/2)/sqrt(square(width/2, height - 24, statX, statY));
    }
    else{
        cos = 0;
        sin = 1;
    }

    do{
        x+=add*cos;
        y-=add*sin;

        if (x >= width-252+28 || x <= 252-28) {
            cos *= -1;
            x += add*cos;
        }
        if(y <= tg_vector[tg_vector.size()-1][0].y || y>=(height-24)){break;}

        for (int i = 0; i < tg_vector.size(); i++) {
            for (int j = 0; j < tg_vector[i].size(); j++) {
                auto &ball = tg_vector[i][j];
                if((square(x,y,ball.x,ball.y) <= (tg_ball.rad*2)*(tg_ball.rad*2)+500) && ball.show && !ball.falling){
                    get_out=true;
                    break;
                }
            }
        }

        filledCircleRGBA(renderer, x, y, 5, 255, 000, 000, 255);
    }while(!get_out);
}

void laserGuide(SDL_Renderer* renderer, bool k){
    SDL_SetRenderDrawColor(renderer, 000, 255, 000, 255);
    double y=height-24, x=width/2, add=1;
    double sin, cos;

    if(k){
        if(statX==0 && statY ==0){
            statX=m_x;
            statY=m_y;
        }
    }
    else{
        statX=m_x;
        statY=m_y;
    }

    if(statX != width/2){
        sin = ((-1)*statY + height - 24)/sqrt(square(width/2, height - 24, statX, statY));
        cos = (statX - width/2)/sqrt(square(width/2, height - 24, statX, statY));
    }
    else{
        cos = 0;
        sin = 1;
    }

    do{
        x+=add*cos;
        y-=add*sin;

        if(y>=(height-24) || y<0 || x>1200 || x<0){break;}

        SDL_RenderDrawPointF(renderer, x, y);
    }while(true);
}

void thunder_anime(){
    Mix_PlayChannel(-1, thunder_chunk, 0);
    int k=0;
    bool bro = false;
    for(int i=0; i<tg_vector.size(); i++){
        for(int j =0; j<tg_vector[i].size(); j++){
            if(tg_vector[i][j].show){
                k=i;
                bro = true;
                break;
            }
        }
        if(bro){
            break;
        }
    }
    if(k+3>tg_vector.size()-1){
        k=tg_vector.size()-1-3;
    }
    for(int i=k; i<k+3; i++){
        for(int j =0; j<tg_vector[i].size(); j++){
            tg_vector[i][j].show = false;
        }
    }
    if(k+3>=tg_vector.size()-1){
        for(int j =0; j<tg_vector[tg_vector.size()-1].size(); j++){
            tg_vector[tg_vector.size()-1][j].show = false;
        }
    }
    terminator(tg_vector);
    sh_vector[0] = sh_vector[1];
    sh_vector.pop_back();
}

void laser(){
    Mix_PlayChannel(-1, laser_chunk, 0);
    bool blah = false;
    double alpha;
    if(m_x==width/2){
        blah=true;
    }else{
        alpha=double((-1)*m_y + height-24)/double(m_x - double(width)/2);
    }
    double cos = (m_x - width / 2) / sqrt(square(m_x, m_y, width / 2, height - 24));
    double x,y;
    if(blah){
        y=height;
    }

    for(int i=0; i<tg_vector.size(); i++){
        for(int j=0; j<tg_vector[i].size(); j++){
            if(blah){
                x=width/2;
                y--;
                if(square(tg_vector[i][j].x, tg_vector[i][j].y, x, y)<(2*24)*(2*24)){
                    tg_vector[i][j].show=false;
                }
            }
            else{
                x=tg_vector[i][j].x;
                y=alpha*(x-double(width)/2)-height+24;
                y*=-1;
                if(fabs((y-tg_vector[i][j].y)*cos) < tg_ball.rad+1){
                    tg_vector[i][j].show=false;
                }
            }
        }
    }
    climber(tg_vector);
    terminator(tg_vector);
    sh_vector[0] = sh_vector[1];
    sh_vector.pop_back();
}

void game_pause(SDL_Renderer* renderer){
    SDL_RenderCopy(renderer, game_setting_window, NULL, &game_window_rect);
    if(game_exit){
        SDL_RenderCopy(renderer, game_exit_window, NULL, &game_window_rect);
        SDL_RenderCopy(renderer, game_exit_yes, NULL, &game_exit_yes_rect);
        text(renderer, game_window_rect.x+120, game_window_rect.y+105, "Are you sure you want to quit?", 000, 000, 000, 255, 0);
    }
    else if(defeat){
        SDL_RenderCopy(renderer, game_exit_window, NULL, &game_window_rect);
        SDL_RenderCopy(renderer, game_exit_yes, NULL, &game_exit_yes_rect);
        text(renderer, game_window_rect.x+95, game_window_rect.y+105, "Your current game score will be lost.", 000, 000, 000, 255, 0);
    }
    else{
        SDL_RenderCopy(renderer, game_defeat, NULL, &game_defeat_rect);
        SDL_RenderCopy(renderer, game_setting_btn, NULL, &game_setting_btn_rect);
        SDL_RenderCopy(renderer, game_exit_btn, NULL, &game_exit_rect);
        SDL_RenderCopy(renderer, chunk_toggle, NULL, &gchunk_toggle_rect);
        SDL_RenderCopy(renderer, music_toggle, NULL, &gmusic_toggle_rect);
        SDL_RenderCopy(renderer, game_music, NULL, &game_music_rect);
    }
    SDL_RenderCopy(renderer, exit_btn, NULL, &game_resume_rect);
}

//normal game mode
void normal(SDL_Renderer* renderer){
    startTicks = SDL_GetTicks();

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
            gNormal = false;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            l_press = true;
        }
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            halt = true;
            statX = 0;
            statY = 0;
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_SPACE && !halt){
                if(!sh_vector[0].launch){
                    swap(sh_vector[0], sh_vector[1]);
                    double x=sh_vector[0].x;
                    sh_vector[0].x = sh_vector[1].x;
                    sh_vector[1].x = x;
                }
            }
            if(event.key.keysym.sym == SDLK_s && !halt && colorfulC>0 && !pro && !laser_power && !thunder_power){
                --colorfulC;
                sh_vector[0].all=true;
            }
            if(event.key.keysym.sym == SDLK_b && !halt && bombC>0 && !pro && !laser_power && !thunder_power){
                --bombC;
                sh_vector[0].bomb=true;
            }
            if(event.key.keysym.sym == SDLK_l && !halt && laserC>0 && !pro && !laser_power && !thunder_power){
                --laserC;
                laser_power=true;
            }
            if(event.key.keysym.sym == SDLK_t && !halt && laserC>0 && !pro && !laser_power && !thunder_power){
                --thunderC;
                thunder_power=true;
            }
        }
    }
    SDL_GetMouseState(&m_x, &m_y);

    if(once){
        mode = 1;
        startTimer = SDL_GetTicks();
        normal_init();
        Mix_PlayMusic(background_music, -1);
        once = false;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, home_bg, NULL, &menu_bg_rect);
    SDL_RenderCopy(renderer, wire, NULL, &wire_rect);


    bool x = false;
    bool y = false;
    if(startTicks-wait<=1500){
        x = true;
    }
    if(halt){
        y = true;
    }


    if(!laser_power){
        guide(renderer, y);
    }
    else{
        laserGuide(renderer, y);
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            laser();
            laser_frame=19;
            laser_power = false;
            l_press=false;
        }
    }

    if(thunder_power){
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            thunder_anime();
            thunder_power = false;
            l_press=false;
        }
    }

    tg_ball_draw(renderer, x, y);

    game_draw(renderer, y);

    ball_stat();

    if(collision(renderer)){
        wait = SDL_GetTicks();
    }

    sh_vec_filler();

    if(game_finish){
        if(win){
            for(int i=0; i<tg_vector.size(); i++){
                for(int j=0; j<tg_vector[j].size(); j++){
                    if(!tg_vector[i][j].show || tg_vector[i][j].falling){
                        temp1++;
                    }
                }
            }

            balls1=temp1;

            temp1 = temp1*10 - 20*(SDL_GetTicks() - startTimer)/1000;
            if(intro){
                temp1+=200;
            }
            if(mid){
                temp1+=500;
            }
            if(pro){
                temp1+=800;
            }

            if(temp1>norm_score){
                norm_score=temp1;
            }
        }

        reInit();
        gNormal = false;
        mode = 1;
        gEnd = true;
        return;
    }

    sh_vec_draw(renderer, y);

    if(halt){
        if(!game_exit){
            game_volume_rect = {game_window_rect.x+10, gchunk_toggle_rect.y, gchunk_toggle_rect.x+20-game_window_rect.x, 40};
            SDL_SetRenderDrawColor(renderer, 255,255,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {gchunk_toggle_rect.x, gchunk_toggle_rect.y, game_window_rect.w+game_window_rect.x-gchunk_toggle_rect.x-20, 40};
            SDL_SetRenderDrawColor(renderer, 000,000,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {game_window_rect.x+10, gmusic_toggle_rect.y, gmusic_toggle_rect.x+20-register_window.x, 40};
            SDL_SetRenderDrawColor(renderer, 255,255,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {gmusic_toggle_rect.x, gmusic_toggle_rect.y, game_window_rect.w+game_window_rect.x-gmusic_toggle_rect.x-20, 40};
            SDL_SetRenderDrawColor(renderer, 000,000,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);
        }

        game_pause(renderer);
        if(button(game_resume_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            if(game_exit){
                game_exit = false;
            }
            else if(defeat){
                defeat = false;
            }
            else{
                halt = false;
            }
            l_press = false;
        }
        if(button(game_exit_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            game_exit = true;
            l_press = false;
        }
        if(button(game_defeat_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            defeat = true;
            l_press = false;
        }
        if(button(game_exit_yes_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && game_exit && !defeat){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gNormal = false;
            gInfinite = false;
            gTimer = false;
            gHardcore = false;
            intro = false;
            mid = false;
            pro = false;
            temp1 = false;
            temp2 = false;
            temp3 = false;
            temp4 = false;
            gMenu = true;
            once = true;
            l_press = false;
            reInit();
            halt = false;
            game_exit = false;
            defeat = false;
            return;
        }
        if(button(game_exit_yes_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && !game_exit && defeat){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gNormal = false;
            gInfinite = false;
            gTimer = false;
            gHardcore = false;
            intro = false;
            mid = false;
            pro = false;
            temp1 = false;
            temp2 = false;
            temp3 = false;
            temp4 = false;
            gEnd = true;
            once = true;
            win = false;
            l_press = false;
            halt = false;
            game_exit = false;
            defeat = false;
            reInit();
            return;
        }

        if(button(game_music_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && !game_exit && !defeat){
            music_sec = true;
            l_press = false;
        }
        if(music_sec){
            SDL_Rect bruh_rect;
            bruh_rect.x = register_window.x-100;
            bruh_rect.y = register_window.y;
            bruh_rect.w = 700;
            bruh_rect.h = 500;
            SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
            SDL_RenderFillRect(renderer, &bruh_rect);
            draw_music(renderer);
        }
        if(music_sec){
            if(button(safari_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!safariBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( safari_music, -1 );
                    safariBool = true;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(frozen_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!frozenBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( frozen_music, -1 );
                    safariBool = false;
                    frozenBool = true;
                    jungleBool = false;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(jungle_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!jungleBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( jungle_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = true;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(pacific_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!pacificBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( pacific_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = true;
                }
                l_press = false;
            }
        }
        if(button(settings_return_rect) && music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            music_sec = false;
            l_press = false;
        }

        if(button(game_music_rect)){
            if(game_music_path == "game/music.png"){
                game_music_path = "game/musicBig.png";
                game_music = IMG_LoadTexture(renderer, game_music_path.c_str());
                game_music_rect = {width/2-280/2-10, game_window_rect.y+146-2, 300, 70};
            }
        }
        else{
            if(game_music_path == "game/musicBig.png") {
                game_music_path = "game/music.png";
                game_music = IMG_LoadTexture(renderer, game_music_path.c_str());
                game_music_rect = {width / 2 - 280 / 2, game_window_rect.y + 146, 280, 65};
            }
        }

        if((button(gchunk_toggle_rect) || button(game_chunk_fissure)) && l_press && !game_exit){
            if(m_x<=game_window_rect.x+game_window_rect.w-100 && m_x>=game_window_rect.x+100){
                gchunk_toggle_rect.x = m_x-20;
                game_pause(renderer);
                if(gchunk_toggle_rect.x<460){
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(0)/300);
                    chunkP=(0)/300;
                }
                else{
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    chunkP=(chunk_toggle_rect.x-450)/300;
                }
            }
        }
        else if(!(button(gmusic_toggle_rect) || button(game_music_fissure))){
            l_press = false;
        }

        if((button(gmusic_toggle_rect) || button(game_music_fissure)) && l_press && !game_exit){
            if(m_x<=game_window_rect.x+game_window_rect.w-100 && m_x>=game_window_rect.x+100){
                gmusic_toggle_rect.x = m_x-20;
                game_pause(renderer);
                if(gchunk_toggle_rect.x<460){
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(0)/300);
                    soundP=(0);
                }
                else{
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    soundP=(chunk_toggle_rect.x-450);
                }
            }
        }
        else if(!(button(gchunk_toggle_rect) || button(game_chunk_fissure))){
            l_press = false;
        }
    }

    elapsedTicks = SDL_GetTicks() - startTicks;
    while (elapsedTicks < desiredDelta) {
        elapsedTicks = SDL_GetTicks() - startTicks;
    }
    SDL_RenderPresent(renderer);
}

void infinite_init(){
    tg_ball.color1='\0';
    tg_ball.color2='\0';
    tg_ball.x=0, tg_ball.y=0;
    tg_ball.visit=false,
    tg_ball.traverse=false,
    tg_ball.show=false,
    tg_ball.falling=false;
    tg_ball.stone=false;
    tg_ball.all=false;
    tg_ball.chain=false;
    sh_ball.color1='\0';
    sh_ball.color2='\0';

    inf_count++;

    for (int i = 0; i < 15; i++) {
        tg_vector.emplace_back(15);
        for (int j = 0; j < 15; j++) {
            if (i % 2 == inf_count%2) {
                tg_vector.back()[j] = {24, 17, '\0', '\0',
                                       252.0 + j * 48, -65.0 - i * 41,
                                       false, false, true, false, false, false, false};
            } else {
                tg_vector.back()[j] = {24, 17, '\0', '\0',
                                       276.0 + j * 48, -65.0 - i * 41,
                                       false, false, true, false, false, false, false};
            }
        }
    }

    for (int i = 0; i < tg_vector.size(); i++) {
        for (int j = 0; j < tg_vector[i].size(); j++) {
            if (tg_vector[i][j].color1 == '\0' && tg_vector[i][j].color2 == '\0') {
                int a = rand() % 5 + 3;
                color(tg_ball.color1, tg_ball.color2);
                pattern(tg_vector, i, j, a, tg_ball.color1, tg_ball.color2);
            }
        }
    }

    int c = rand()%15+15*(inf_count);
    int d = rand()%15;
    prison(tg_vector, c, d);

    for(int i=0; i<7; i++){
        int n = rand()%5+5;
        int a=rand()%15+15*(inf_count);
        if(a<15*(inf_count)+4){
            a+=2;
        }
        int b = rand()%15;
        recolor(tg_vector, a, b, n);
    }

    int a = rand()%15+15*(inf_count);
    int b = rand()%15;
    tg_vector[a][b].stone=true;
}

void infinite(SDL_Renderer* renderer){
    startTicks = SDL_GetTicks();

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
            gInfinite = false;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            l_press = true;
        }
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            halt = true;
            statX = 0;
            statY = 0;
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_SPACE && !halt){
                if(!sh_vector[0].launch){
                    swap(sh_vector[0], sh_vector[1]);
                    double x=sh_vector[0].x;
                    sh_vector[0].x = sh_vector[1].x;
                    sh_vector[1].x = x;
                }
            }
            if(event.key.keysym.sym == SDLK_s && !halt && colorfulC>0 && !pro && !laser_power && !thunder_power){
                --colorfulC;
                sh_vector[0].all=true;
            }
            if(event.key.keysym.sym == SDLK_b && !halt && bombC>0 && !pro && !laser_power && !thunder_power){
                --bombC;
                sh_vector[0].bomb=true;
            }
            if(event.key.keysym.sym == SDLK_l && !halt && laserC>0 && !pro && !laser_power && !thunder_power){
                --laserC;
                laser_power=true;
            }
            if(event.key.keysym.sym == SDLK_t && !halt && laserC>0 && !pro && !laser_power && !thunder_power){
                --thunderC;
                thunder_power=true;
            }
        }
    }
    SDL_GetMouseState(&m_x, &m_y);

    if(once){
        mode = 2;
        startTimer = SDL_GetTicks();
        Mix_PlayMusic(background_music, -1);
        normal_init();
        once = false;
    }

    int count=0;
    for(int i=0; i<tg_vector.size(); i++){
        for(int j=0; j<tg_vector[i].size(); j++){
            if(tg_vector[i][j].show || !tg_vector[i][j].falling){
                count++;
            }
        }
    }
    if(tg_vector[tg_vector.size()-1][0].y >= -24 || count==0){
        infinite_init();
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, home_bg, NULL, &menu_bg_rect);
    SDL_RenderCopy(renderer, wire, NULL, &wire_rect);

    bool x = false;
    bool y = false;
    if(startTicks-wait<=1500){
        x = true;
    }
    if(halt){
        y = true;
    }

    if(!laser_power){
        guide(renderer, y);
    }
    else{
        laserGuide(renderer, y);
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            laser();
            laser_frame=19;
            laser_power = false;
            l_press=false;
        }
    }

    if(thunder_power){
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            thunder_anime();
            thunder_power = false;
            l_press=false;
        }
    }

    tg_ball_draw(renderer, x, y);

    ball_stat();

    game_draw(renderer, y);

    if(collision(renderer)){
        wait = SDL_GetTicks();
    }

    sh_vec_filler();

    if(game_finish){
        win=true;
        if(win){
            for(int i=0; i<tg_vector.size(); i++){
                for(int j=0; j<tg_vector[j].size(); j++){
                    if(!tg_vector[i][j].show || tg_vector[i][j].falling){
                        temp2++;
                    }
                }
            }

            balls2=temp2;

            temp2 = temp2*15 - 20*(SDL_GetTicks() - startTimer)/1000;
            if(intro){
                temp2+=200;
            }
            if(mid){
                temp2+=500;
            }
            if(pro){
                temp2+=800;
            }

            if(temp2>norm_score){
                inf_score=temp2;
            }
        }

        reInit();
        gInfinite = false;
        mode = 2;
        gEnd = true;
        return;
    }

    sh_vec_draw(renderer, y);

    if(halt){
        if(!game_exit){
            game_volume_rect = {game_window_rect.x+10, gchunk_toggle_rect.y, gchunk_toggle_rect.x+20-game_window_rect.x, 40};
            SDL_SetRenderDrawColor(renderer, 255,255,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {gchunk_toggle_rect.x, gchunk_toggle_rect.y, game_window_rect.w+game_window_rect.x-gchunk_toggle_rect.x-20, 40};
            SDL_SetRenderDrawColor(renderer, 000,000,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {game_window_rect.x+10, gmusic_toggle_rect.y, gmusic_toggle_rect.x+20-register_window.x, 40};
            SDL_SetRenderDrawColor(renderer, 255,255,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {gmusic_toggle_rect.x, gmusic_toggle_rect.y, game_window_rect.w+game_window_rect.x-gmusic_toggle_rect.x-20, 40};
            SDL_SetRenderDrawColor(renderer, 000,000,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);
        }

        game_pause(renderer);
        if(button(game_resume_rect) && !music && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            if(game_exit){
                game_exit = false;
            }
            else if(defeat){
                defeat = false;
            }
            else{
                halt = false;
            }
            l_press = false;
        }
        if(button(game_exit_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            game_exit = true;
            l_press = false;
        }
        if(button(game_defeat_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            defeat = true;
            l_press = false;
        }
        if(button(game_exit_yes_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && game_exit && !defeat){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gNormal = false;
            gInfinite = false;
            gTimer = false;
            gHardcore = false;
            intro = false;
            mid = false;
            pro = false;
            temp1 = false;
            temp2 = false;
            temp3 = false;
            temp4 = false;
            gMenu = true;
            once = true;
            l_press = false;
            halt = false;
            game_exit = false;
            defeat = false;
            reInit();
            return;
        }
        if(button(game_exit_yes_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && !game_exit && defeat){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gNormal = false;
            gInfinite = false;
            gTimer = false;
            gHardcore = false;
            intro = false;
            mid = false;
            pro = false;
            temp1 = false;
            temp2 = false;
            temp3 = false;
            temp4 = false;
            gEnd = true;
            once = true;
            win = false;
            l_press = false;
            halt = false;
            game_exit = false;
            defeat = false;
            reInit();
            return;
        }
        if(button(game_music_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && !game_exit && !defeat){
            music_sec = true;
            l_press = false;
        }
        if(music_sec){
            SDL_Rect bruh_rect;
            bruh_rect.x = register_window.x-100;
            bruh_rect.y = register_window.y;
            bruh_rect.w = 700;
            bruh_rect.h = 500;
            SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
            SDL_RenderFillRect(renderer, &bruh_rect);
            draw_music(renderer);
        }
        if(music_sec){
            if(button(safari_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!safariBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( safari_music, -1 );
                    safariBool = true;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(frozen_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!frozenBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( frozen_music, -1 );
                    safariBool = false;
                    frozenBool = true;
                    jungleBool = false;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(jungle_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!jungleBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( jungle_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = true;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(pacific_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!pacificBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( pacific_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = true;
                }
                l_press = false;
            }
        }
        if(button(settings_return_rect) && music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            music_sec = false;
            l_press = false;
        }

        if(button(game_music_rect)){
            if(game_music_path == "game/music.png"){
                game_music_path = "game/musicBig.png";
                game_music = IMG_LoadTexture(renderer, game_music_path.c_str());
                game_music_rect = {width/2-280/2-10, game_window_rect.y+146-2, 300, 70};
            }
        }
        else{
            if(game_music_path == "game/musicBig.png") {
                game_music_path = "game/music.png";
                game_music = IMG_LoadTexture(renderer, game_music_path.c_str());
                game_music_rect = {width / 2 - 280 / 2, game_window_rect.y + 146, 280, 65};
            }
        }

        if((button(gchunk_toggle_rect) || button(game_chunk_fissure)) && l_press && !game_exit){
            if(m_x<=game_window_rect.x+game_window_rect.w-100 && m_x>=game_window_rect.x+100){
                gchunk_toggle_rect.x = m_x-20;
                game_pause(renderer);
                if(gchunk_toggle_rect.x<460){
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(0)/300);
                    chunkP=(0)/300;
                }
                else{
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    chunkP=(chunk_toggle_rect.x-450)/300;
                }
            }
        }
        else if(!(button(gmusic_toggle_rect) || button(game_music_fissure))){
            l_press = false;
        }

        if((button(gmusic_toggle_rect) || button(game_music_fissure)) && l_press && !game_exit){
            if(m_x<=game_window_rect.x+game_window_rect.w-100 && m_x>=game_window_rect.x+100){
                gmusic_toggle_rect.x = m_x-20;
                game_pause(renderer);
                if(gchunk_toggle_rect.x<460){
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(0)/300);
                    soundP=(0);
                }
                else{
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    soundP=(chunk_toggle_rect.x-450);
                }
            }
        }
        else if(!(button(gchunk_toggle_rect) || button(game_chunk_fissure))){
            l_press = false;
        }
    }

    elapsedTicks = SDL_GetTicks() - startTicks;
    while (elapsedTicks < desiredDelta) {
        elapsedTicks = SDL_GetTicks() - startTicks;
    }
    SDL_RenderPresent(renderer);
}

void timer(SDL_Renderer* renderer){
    startTicks = SDL_GetTicks();

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
            gTimer = false;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            l_press = true;
        }
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            halt = true;
            statX = 0;
            statY = 0;
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_SPACE && !halt){
                if(!sh_vector[0].launch){
                    swap(sh_vector[0], sh_vector[1]);
                    double x=sh_vector[0].x;
                    sh_vector[0].x = sh_vector[1].x;
                    sh_vector[1].x = x;
                }
            }
            if(event.key.keysym.sym == SDLK_s && !halt && colorfulC>0 && !pro && !laser_power && !thunder_power){
                --colorfulC;
                sh_vector[0].all=true;
            }
            if(event.key.keysym.sym == SDLK_b && !halt && bombC>0 && !pro && !laser_power && !thunder_power){
                --bombC;
                sh_vector[0].bomb=true;
            }
            if(event.key.keysym.sym == SDLK_l && !halt && laserC>0 && !pro && !laser_power && !thunder_power){
                --laserC;
                laser_power=true;
            }
            if(event.key.keysym.sym == SDLK_t && !halt && laserC>0 && !pro && !laser_power && !thunder_power){
                --thunderC;
                thunder_power=true;
            }
        }
    }
    SDL_GetMouseState(&m_x, &m_y);

    if(once){
        mode = 3;
        startTimer = SDL_GetTicks();
        normal_init();
        Mix_PlayMusic(background_music, -1);
        once = false;
    }
    if(SDL_GetTicks()-startTimer>60000){
        game_finish = true;
        win = false;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, home_bg, NULL, &menu_bg_rect);
    SDL_RenderCopy(renderer, wire, NULL, &wire_rect);

    bool x = false;
    bool y = false;
    if(startTicks-wait<=1500){
        x = true;
    }
    if(halt){
        y = true;
    }

    if(!laser_power){
        guide(renderer, y);
    }
    else{
        laserGuide(renderer, y);
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            laser();
            laser_frame=19;
            laser_power = false;
            l_press=false;
        }
    }

    if(thunder_power){
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            thunder_anime();
            thunder_power = false;
            l_press=false;
        }
    }

    tg_ball_draw(renderer, x, y);

    game_draw(renderer, y);

    text(renderer, 75, 50, to_string(60000/1000-(SDL_GetTicks()-startTimer)/1000).c_str(), 000, 000, 000, 255, 2);

    ball_stat();

    if(collision(renderer)){
        wait = SDL_GetTicks();
    }

    sh_vec_filler();

    if(game_finish){
        if(win){
            for(int i=0; i<tg_vector.size(); i++){
                for(int j=0; j<tg_vector[j].size(); j++){
                    if(!tg_vector[i][j].show || tg_vector[i][j].falling){
                        temp3++;
                    }
                }
            }

            balls3=temp3;

            temp3 = temp3*20 - 20*(SDL_GetTicks() - startTimer)/1000;
            if(intro){
                temp3+=200;
            }
            if(mid){
                temp3+=500;
            }
            if(pro){
                temp3+=800;
            }

            if(temp3>norm_score){
                norm_score=temp3;
            }
        }

        reInit();
        gTimer = false;
        mode = 3;
        gEnd = true;
        return;
    }

    sh_vec_draw(renderer, y);

    if(halt){
        if(!game_exit){
            game_volume_rect = {game_window_rect.x+10, gchunk_toggle_rect.y, gchunk_toggle_rect.x+20-game_window_rect.x, 40};
            SDL_SetRenderDrawColor(renderer, 255,255,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {gchunk_toggle_rect.x, gchunk_toggle_rect.y, game_window_rect.w+game_window_rect.x-gchunk_toggle_rect.x-20, 40};
            SDL_SetRenderDrawColor(renderer, 000,000,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {game_window_rect.x+10, gmusic_toggle_rect.y, gmusic_toggle_rect.x+20-register_window.x, 40};
            SDL_SetRenderDrawColor(renderer, 255,255,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);

            game_volume_rect = {gmusic_toggle_rect.x, gmusic_toggle_rect.y, game_window_rect.w+game_window_rect.x-gmusic_toggle_rect.x-20, 40};
            SDL_SetRenderDrawColor(renderer, 000,000,000,255);
            SDL_RenderFillRect(renderer, &game_volume_rect);
        }

        game_pause(renderer);
        if(button(game_resume_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            if(game_exit){
                game_exit = false;
            }
            else if(defeat){
                defeat = false;
            }
            else{
                halt = false;
            }
            l_press = false;
        }
        if(button(game_exit_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            game_exit = true;
            l_press = false;
        }
        if(button(game_defeat_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            defeat = true;
            l_press = false;
        }
        if(button(game_exit_yes_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && game_exit && !defeat){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gNormal = false;
            gInfinite = false;
            gTimer = false;
            gHardcore = false;
            intro = false;
            mid = false;
            pro = false;
            temp1 = false;
            temp2 = false;
            temp3 = false;
            temp4 = false;
            gMenu = true;
            once = true;
            l_press = false;
            halt = false;
            game_exit = false;
            defeat = false;
            reInit();
            return;
        }
        if(button(game_exit_yes_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && !game_exit && defeat){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            gNormal = false;
            gInfinite = false;
            gTimer = false;
            gHardcore = false;
            intro = false;
            mid = false;
            pro = false;
            temp1 = false;
            temp2 = false;
            temp3 = false;
            temp4 = false;
            gEnd = true;
            once = true;
            win = false;
            l_press = false;
            halt = false;
            game_exit = false;
            defeat = false;
            reInit();
            return;
        }
        if(button(game_music_rect) && !music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press && !game_exit && !defeat){
            music_sec = true;
            l_press = false;
        }
        if(music_sec){
            SDL_Rect bruh_rect;
            bruh_rect.x = register_window.x-100;
            bruh_rect.y = register_window.y;
            bruh_rect.w = 700;
            bruh_rect.h = 500;
            SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
            SDL_RenderFillRect(renderer, &bruh_rect);
            draw_music(renderer);
        }
        if(music_sec){
            if(button(safari_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!safariBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( safari_music, -1 );
                    safariBool = true;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(frozen_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!frozenBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( frozen_music, -1 );
                    safariBool = false;
                    frozenBool = true;
                    jungleBool = false;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(jungle_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!jungleBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( jungle_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = true;
                    pacificBool = false;
                }
                l_press = false;
            }
            if(button(pacific_rect) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
                if(!pacificBool){
                    Mix_HaltMusic();
                    Mix_PlayMusic( pacific_music, -1 );
                    safariBool = false;
                    frozenBool = false;
                    jungleBool = false;
                    pacificBool = true;
                }
                l_press = false;
            }
        }
        if(button(settings_return_rect) && music_sec && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && l_press){
            Mix_PlayChannel(-1, toggle_chunk, 0);
            music_sec = false;
            l_press = false;
        }
        if(button(game_music_rect)){
            if(game_music_path == "game/music.png"){
                game_music_path = "game/musicBig.png";
                game_music = IMG_LoadTexture(renderer, game_music_path.c_str());
                game_music_rect = {width/2-280/2-10, game_window_rect.y+146-2, 300, 70};
            }
        }
        else{
            if(game_music_path == "game/musicBig.png") {
                game_music_path = "game/music.png";
                game_music = IMG_LoadTexture(renderer, game_music_path.c_str());
                game_music_rect = {width / 2 - 280 / 2, game_window_rect.y + 146, 280, 65};
            }
        }

        if((button(gchunk_toggle_rect) || button(game_chunk_fissure)) && l_press && !game_exit){
            if(m_x<=game_window_rect.x+game_window_rect.w-100 && m_x>=game_window_rect.x+100){
                gchunk_toggle_rect.x = m_x-20;
                game_pause(renderer);
                if(gchunk_toggle_rect.x<460){
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(0)/300);
                    chunkP=(0)/300;
                }
                else{
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(bomb_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(ball_explosion_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(laser_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeChunk(thunder_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    chunkP=(chunk_toggle_rect.x-450)/300;
                }
            }
        }
        else if(!(button(gmusic_toggle_rect) || button(game_music_fissure))){
            l_press = false;
        }

        if((button(gmusic_toggle_rect) || button(game_music_fissure)) && l_press && !game_exit){
            if(m_x<=game_window_rect.x+game_window_rect.w-100 && m_x>=game_window_rect.x+100){
                gmusic_toggle_rect.x = m_x-20;
                game_pause(renderer);
                if(gchunk_toggle_rect.x<460){
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(0)/300);
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(0)/300);
                    soundP=(0);
                }
                else{
                    Mix_VolumeChunk(toggle_chunk,MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    Mix_VolumeMusic(MIX_MAX_VOLUME*(chunk_toggle_rect.x-450)/300);
                    soundP=(chunk_toggle_rect.x-450);
                }
            }
        }
        else if(!(button(gchunk_toggle_rect) || button(game_chunk_fissure))){
            l_press = false;
        }
    }

    elapsedTicks = SDL_GetTicks() - startTicks;
    while (elapsedTicks < desiredDelta) {
        elapsedTicks = SDL_GetTicks() - startTicks;
    }
    SDL_RenderPresent(renderer);
}