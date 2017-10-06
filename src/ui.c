#include "ui.h"
#include "control.h"

SDL_Texture *UI_RenderLines(const char *message, SDL_Color *c) {
    int w = 0;
    int max_w = 0;
    int lines = 1;
    SDL_Surface *surf;
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\n') {
            lines++;
            max_w = 0;
        }
        else {
            if (glyphs[message[i]] == NULL) {
                glyphs[message[i]] = TTF_RenderGlyph_Solid(UI_Font, message[i], *c);
            }
            max_w += glyphs[message[i]]->w;
            if (max_w > w) {
                w = max_w;
            }
        }
    }
    int line_height = TTF_FontHeight(UI_Font);
    surf = SDL_CreateRGBSurfaceWithFormat(0, w, lines*line_height, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Rect r = (SDL_Rect) {0, 0, 0, 0};
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\n') {
            r.y += line_height;
            r.x = 0;
        }
        else {
            r.w = glyphs[message[i]]->w;
            r.h = glyphs[message[i]]->h;
            SDL_BlitSurface(glyphs[message[i]], NULL, surf, &r);
            r.x += r.w;
        }
    }
    SDL_Texture *text = SDL_CreateTextureFromSurface(UI_Renderer, surf);
    SDL_FreeSurface(surf);
    return text;
}

SDL_Texture *UI_LoadImage(const char *file) {
    SDL_Surface *surf = IMG_Load(file);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(UI_Renderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}

char *UI_Add(char *left, const char *right) {
    size_t l = strlen(left);
    size_t r = strlen(right);
    char *ret = malloc(sizeof(char)*(l+r+1));
    for (int i = 0; i < l; i++) {
        ret[i] = left[i];
    }
    for (int i = 0; i < r; i++) {
        ret[l+i] = right[i];
    }
    ret[l+r] = '\0';
    return ret;
}

UI_Element *UI_NewEmpty(SDL_Color *bg, SDL_Color *fg, UI_Element *parent) {
    UI_Element *element = malloc(sizeof(UI_Element));
    element->current = bg;
    element->background = bg;
    element->foreground = fg;
    element->parent = parent;
    element->type = UI_EMPTY;
    element->align = 0;
    element->scale = UI_SCALE;
    element->element = NULL;
    if (parent) {
        UI_Layout *l = parent->element;
        vInsert(l->elements, element);
    }
    UI_Focus = element;
    return element;
}

UI_Element *UI_NewImage(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, SDL_Texture *img) {
    UI_Element *element = UI_NewEmpty(bg, fg, parent);
    element->type = UI_IMAGE;

    UI_Image *image = malloc(sizeof(UI_Image));
    image->img = img;

    element->element = image;
    return element;
}

UI_Element *UI_NewLayout(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, UI_layout_t type, Uint8 margin) {
    UI_Element *element = UI_NewEmpty(bg, fg, parent);
    element->type = UI_LAYOUT;

    UI_Layout *layout = malloc(sizeof(UI_Layout));
    layout->type = type;
    layout->margin = margin;
    layout->elements = newVector();

    element->element = layout;
    return element;
}

UI_Element *UI_NewButton(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, SDL_Texture *img, void (*onClick)()) {
    UI_Element *element = UI_NewEmpty(bg, fg, parent);
    element->type = UI_BUTTON;

    UI_Button *button = malloc(sizeof(UI_Button));
    button->img = img;
    button->onClick = onClick;

    element->element = button;
    return element;
}

UI_Element *UI_NewTextBox(SDL_Color *bg, SDL_Color *fg, UI_Element *parent, const char *text, SDL_bool editable, SDL_bool multiline) {
    UI_Element *element = UI_NewEmpty(bg, fg, parent);
    element->type = UI_TEXTBOX;

    UI_TextBox *textbox = malloc(sizeof(UI_TextBox));
    textbox->text = malloc(sizeof(char)*(strlen(text)+1));
    strcpy(textbox->text, text);
    textbox->img = UI_RenderLines(textbox->text, fg);
    textbox->editable = editable;
    textbox->multiline = multiline;

    element->element = textbox;
    return element;
}

void UI_FreeElement(UI_Element *element) {
    UI_Layout *l;
    vector *v;
    int i;
    switch (element->type) {
    case UI_TEXTBOX:
    free(((UI_TextBox*)element->element)->text);
    SDL_DestroyTexture(((UI_TextBox*)element->element)->img);
    case UI_IMAGE:
    case UI_BUTTON:
        free(element->element);
    case UI_EMPTY:
        free(element);
        break;
    case UI_LAYOUT:
        l = element->element;
        v = l->elements;
        for (i = 0; i < v->count; i++) {
            UI_FreeElement(vGet(v, i));
        }
        deleteVector(v);
        free(l);
        free(element);
    }
}

void UI_MapLayout(UI_Layout *l, SDL_Rect *rect, void (*func)(UI_Element *e, SDL_Rect *rect, void *param), void *param) {
    SDL_Rect *r = malloc(sizeof(SDL_Rect));
    for (int i = 0; i < l->elements->count; i++) {
        memcpy(r, rect, sizeof(SDL_Rect));
        if (l->type == UI_VERT) {
            r->h /= l->elements->count;
            r->h -= l->margin * (l->elements->count - 1) / l->elements->count;
            r->y += i*(r->h + l->margin);
        }
        else if (l->type == UI_HORIZ) {
            r->w /= l->elements->count;
            r->w -= l->margin * (l->elements->count - 1) / l->elements->count;
            r->x += i*(r->w + l->margin);
        }
        func(vGet(l->elements, i), r, param);
    }
    free(r);
}

void UI_Draw(UI_Element *root) {
    int w, h;
    SDL_GetRendererOutputSize(UI_Renderer, &w, &h);
    SDL_Rect window_rect = {0, 0, w, h};
    UI_DrawElement(root, &window_rect, NULL);
}

void UI_DrawElement(UI_Element *e, SDL_Rect *rect, void *thing) {
    SDL_SetRenderDrawColor(UI_Renderer, e->current->r, e->current->g, e->current->b, e->current->a);
    SDL_RenderFillRect(UI_Renderer, rect);
    SDL_SetRenderDrawColor(UI_Renderer, 0, 0, 0, 255);
    if (e->type == UI_IMAGE || e->type == UI_BUTTON || e->type == UI_TEXTBOX) {
        SDL_Texture *text;
        if (e->type == UI_IMAGE) {
            text = ((UI_Image*)e->element)->img;
        }
        else if (e->type == UI_BUTTON) {
            text = ((UI_Button*)e->element)->img;
        }
        else if (e->type == UI_TEXTBOX) {
            text = ((UI_TextBox*)e->element)->img;
        }
        int x, y;
        int w, h;
        if (e->scale == UI_KEEP || e->scale == UI_SCALE) {
            SDL_QueryTexture(text, NULL, NULL, &w, &h);
        }
        else {
            w = rect->w;
            h = rect->h;
        }
        if (e->scale == UI_SCALE) {
            if ((w > h && rect->w > rect->h) || (h >= w && rect->h >= rect->w)) {
                if (w/(float)h > rect->w/(float)rect->h) {
                    h = (rect->w * h) / w;
                    w = rect->w;
                }
                else {
                    w = (rect->h * w) / h;
                    h = rect->h;
                }
            }
            else if (w > h && rect->h >= rect->w) {
                h = (rect->w * h) / w;
                w = rect->w;
            }
            else if (h >= w && rect->w > rect->h) {
                w = (rect->h * w) / h;
                h = rect->h;
            }
        }
        else if (e->scale == UI_STRETCH) {
            w = rect->w;
            h = rect->h;
        }
        if (e->align & UI_LEFT) {
            x = rect->x;
        }
        else if (e->align & UI_RIGHT) {
            x = rect->x + rect->w - w;
        }
        else {
            x = rect->x + (rect->w - w)/2;
        }
        if (e->align & UI_TOP) {
            y = rect->y;
        }
        else if (e->align & UI_BOTTOM) {
            y = rect->y + rect->h - h;
        }
        else {
            y = rect->y + (rect->h - h)/2;
        }
        SDL_Rect r = (SDL_Rect) {x, y, w, h};
        SDL_RenderCopy(UI_Renderer, text, NULL, &r);
    }
    else if (e->type == UI_LAYOUT) {
        UI_MapLayout(e->element, rect, UI_DrawElement, NULL);
    }
}

void UI_Update(UI_Element *root, Uint32 *interval) {
    int w, h;
    SDL_GetRendererOutputSize(UI_Renderer, &w, &h);
    SDL_Rect window_rect = {0, 0, w, h};
    UI_UpdateElement(root, &window_rect, interval);
}

void UI_UpdateElement(UI_Element *e, SDL_Rect *rect, Uint32 *interval) {
    if (e->type == UI_LAYOUT) {
        UI_MapLayout(e->element, rect, UI_UpdateElement, interval);
    }
    else if (UI_Focus == e && e->type != UI_EMPTY) {
        e->current = UI_FocusColor;
    }
    else {
        e->current = e->background;
    }
}

void UI_Respond(UI_Element *root, SDL_Event *evt) {
    int w, h;
    SDL_GetRendererOutputSize(UI_Renderer, &w, &h);
    SDL_Rect window_rect = {0, 0, w, h};
    UI_RespondElement(root, &window_rect, evt);
}

void UI_RespondElement(UI_Element *e, SDL_Rect *rect, SDL_Event *evt) {
    if (e->type == UI_BUTTON) {
        UI_Button *button = e->element;
        UI_Select.touch = rect;
        if (e == UI_Focus && checkInputEvent(evt, &UI_Select)) {
            button->onClick();
        }
        UI_Select.touch = NULL;
    }
    else if (e->type == UI_LAYOUT) {
        UI_MapLayout(e->element, rect, UI_RespondElement, evt);
    }
    if (e->type != UI_LAYOUT) {
        SDL_bool left = checkInputEvent(evt, &UI_Left);
        SDL_bool right = checkInputEvent(evt, &UI_Right);
        SDL_bool up = checkInputEvent(evt, &UI_Up);
        SDL_bool down = checkInputEvent(evt, &UI_Down);
        if (e->parent && e == UI_Focus && (left || right || up || down)) {
            UI_Element *top = e->parent;
            UI_Layout *l = top->element;
            UI_Element *bottom = e;
            while ((up || down) && l->type == UI_HORIZ || (left || right) && l->type == UI_VERT) {
                bottom = top;
                top = top->parent;
                if (top) {
                    l = top->element;
                }
                else {
                    l = NULL;
                    break;
                }
            }
            if (l) {
                int diff = 0;
                if (up || left) diff = -1;
                else if (down || right) diff = 1;
                int index = vFind(l->elements, bottom);
                int temp = index;
                index = (index + diff) % l->elements->count;
                if (index < 0) index += l->elements->count;
                UI_Element *thing = vGet(l->elements, index);
                while (thing->type == UI_EMPTY && index != temp) {
                    index = (index + diff) % l->elements->count;
                    if (index < 0) index += l->elements->count;
                    thing = vGet(l->elements, index);
                }
                while (thing->type == UI_LAYOUT) {
                    l = thing->element;
                    thing = vGet(l->elements, 0);
                }
                evt->type = 0; //TODO: fix this
                UI_Focus = thing;
            }
        }
        if (evt->type == SDL_KEYDOWN) {
            SDL_Keycode key = evt->key.keysym.sym;
            if (e == UI_Focus && e->type == UI_TEXTBOX) {
                UI_TextBox *box = e->element;
                if (box->editable) {
                    if (SDL_GetModState() & KMOD_CTRL) {
                        if (key == SDLK_c) {
                            SDL_SetClipboardText(box->text);
                        }
                        else if (key == SDLK_v) {
                            box->text = SDL_GetClipboardText();
                        }
                    }
                    else if (key == SDLK_BACKSPACE) {
                        box->text[strlen(box->text)-1] = '\0';
                    }
                    else if (key == SDLK_RETURN && box->multiline) {
                        char *x = UI_Add(box->text, "\n");
                        free(box->text);
                        box->text = x;
                    }
                    SDL_DestroyTexture(box->img);
                    box->img = UI_RenderLines(box->text, e->foreground);
                }
            }
        }
        else if (evt->type == SDL_MOUSEMOTION) {
            Sint32 x = evt->motion.x;
            Sint32 y = evt->motion.y;
            if (x >= rect->x && x <= rect->x + rect->w &&
                y >= rect->y && y <= rect->y + rect->h) {
                UI_Focus = e;
            }
        }
        else if (evt->type == SDL_FINGERMOTION) {
            Sint32 x = evt->tfinger.x;
            Sint32 y = evt->tfinger.y;
            if (x >= rect->x && x <= rect->x + rect->w &&
                y >= rect->y && y <= rect->y + rect->h) {
                UI_Focus = e;
            }
        }
        else if (e->type == UI_TEXTBOX && e == UI_Focus && evt->type == SDL_TEXTINPUT) {
            UI_TextBox *box = e->element;
            if (box->editable) {
                char *x = UI_Add(box->text, evt->text.text);
                free(box->text);
                box->text = x;
                SDL_DestroyTexture(box->img);
                box->img = UI_RenderLines(box->text, e->foreground);
            }
        }
    }
}

void UI_Init(SDL_Renderer *renderer, TTF_Font *font, SDL_Color *focus) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    UI_Renderer = renderer;
    UI_Font = font;
    UI_Focus = NULL;
    UI_FocusColor = malloc(sizeof(SDL_Color));
    controller_t *controller_select = malloc(sizeof(controller_t));
    controller_t *controller_left = malloc(sizeof(controller_t));
    controller_t *controller_right = malloc(sizeof(controller_t));
    controller_t *controller_up = malloc(sizeof(controller_t));
    controller_t *controller_down = malloc(sizeof(controller_t));
    *controller_select = (controller_t) {0, -1, -1, -1};
    *controller_left = (controller_t) {-1, SDL_HAT_LEFT, -1, -1};
    *controller_right = (controller_t) {-1, SDL_HAT_RIGHT, -1, -1};
    *controller_up = (controller_t) {-1, SDL_HAT_UP, -1, -1};
    *controller_down = (controller_t) {-1, SDL_HAT_DOWN, -1, -1};
    UI_Select = (control_t) {SDL_SCANCODE_RETURN, controller_select, NULL};
    UI_Left = (control_t) {SDL_SCANCODE_LEFT, controller_left, NULL};
    UI_Right = (control_t) {SDL_SCANCODE_RIGHT, controller_right, NULL};
    UI_Up = (control_t) {SDL_SCANCODE_UP, controller_up, NULL};
    UI_Down = (control_t) {SDL_SCANCODE_DOWN, controller_down, NULL};
    if (focus) {
        memcpy(UI_FocusColor, focus, sizeof(SDL_Color));
    }
    else {
        SDL_Color c = {128, 128, 128, 128};
        memcpy(UI_FocusColor, &c, sizeof(SDL_Color));
    }
    SDL_StartTextInput();
}

void UI_Quit() {
    SDL_StopTextInput();
    free(UI_FocusColor);
    free(UI_Select.controller);
    free(UI_Left.controller);
    free(UI_Right.controller);
    free(UI_Up.controller);
    free(UI_Down.controller);
}
