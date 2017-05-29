#ifndef UI_H
#define UI_H

#include "vector.h"
#include "control.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

typedef enum {
    UI_BUTTON,
    UI_LAYOUT,
    UI_IMAGE,
    UI_EMPTY,
    UI_TEXTBOX,
} UI_Element_t;

typedef enum {
    UI_VERT,
    UI_HORIZ,
} UI_layout_t;

typedef enum {
    UI_TOP = 1,
    UI_RIGHT = 2,
    UI_BOTTOM = 4,
    UI_LEFT = 8
} UI_align_t;

typedef enum {
    UI_STRETCH,
    UI_SCALE,
    UI_KEEP
} UI_scale_t;

typedef struct {
    SDL_Texture *img;
} UI_Image;

typedef struct {
    UI_layout_t type;
    Uint8 margin;
    vector *elements;
} UI_Layout;

typedef struct {
    SDL_Texture *img;
    void (*onClick)();
} UI_Button;

typedef struct {
    char *text;
    SDL_Texture *img;
    SDL_bool editable;
    SDL_bool multiline;
} UI_TextBox;

typedef struct UI_Element UI_Element;

struct UI_Element {
    SDL_Color* current;
    SDL_Color* background;
    SDL_Color* foreground;
    UI_Element *parent;
    UI_Element_t type;
    UI_align_t align;
    UI_scale_t scale;
    void *element;
};

SDL_Texture *UI_RenderLines(const char *message, SDL_Color *c);
SDL_Texture *UI_LoadImage(const char *file);

char *UI_Add(char *left, const char *right);

UI_Element *UI_NewEmpty(SDL_Color *bg, SDL_Color *fg, UI_Element *parent);
UI_Element *UI_NewImage(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, SDL_Texture *img);
UI_Element *UI_NewLayout(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, UI_layout_t type, Uint8 margin);
UI_Element *UI_NewButton(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, SDL_Texture *img, void (*onClick)());
UI_Element *UI_NewTextBox(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, const char *text, SDL_bool editable, SDL_bool multiline);

void UI_FreeElement(UI_Element *element);
void UI_FreeEmpty(UI_Element *element);
void UI_FreeImage(UI_Element *element);
void UI_FreeLayout(UI_Element *element);
void UI_FreeButton(UI_Element *element);
void UI_FreeTextBox(UI_Element *element);

void UI_Draw(UI_Element *root);
void UI_DrawElement(UI_Element *e, SDL_Rect *rect, void *thing);
void UI_Update(UI_Element *root, Uint32 *interval);
void UI_UpdateElement(UI_Element *e, SDL_Rect *rect, Uint32 *interval);
void UI_Respond(UI_Element *root, SDL_Event *evt);
void UI_RespondElement(UI_Element *e, SDL_Rect *rect, SDL_Event *evt);

void UI_Init(SDL_Renderer *renderer, TTF_Font *font, SDL_Color *focused);
void UI_Quit();

SDL_Renderer *UI_Renderer;
TTF_Font *UI_Font;
UI_Element *UI_Focus;
SDL_Color *UI_FocusColor;
control_t UI_Select;
control_t UI_Left;
control_t UI_Right;
control_t UI_Up;
control_t UI_Down;

#endif /*UI_H*/
