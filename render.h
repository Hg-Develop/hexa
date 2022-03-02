/*
 *  作者 芳賀七海
 *  copyright (c) 2020 芳賀七海 All rights reserved.
 */

#ifndef RENDER_H
#define RENDER_H

#include <stdlib.h>
#include <GL/glut.h>

typedef struct {
  struct {
    int x;
    int y;
  } position;
  struct {
    int w;
    int h;
  } size;
  void (*func_display)(void);
  void (*func_mouse)(int, int, int, int);
  void (*func_motion)(int, int);
  void (*func_passiveMotion)(int, int);
  void (*func_keyboard)(unsigned char, int, int);
  void (*func_special)(int, int, int);
} RENDERMETA_STRUCT_;

typedef struct RENDERMETA_STRUCT_LIST_ELEMENT_ {
  RENDERMETA_STRUCT_ data;
  struct RENDERMETA_STRUCT_LIST_ELEMENT_ *next;
  struct RENDERMETA_STRUCT_LIST_ELEMENT_ *prev;
} RENDERMETA_STRUCT_LIST_E_;

typedef struct {
  RENDERMETA_STRUCT_LIST_E_ *head;
  RENDERMETA_STRUCT_LIST_E_ *tail;
} RENDERMETA_STRUCT_LIST_;

#define RENDERMETA_DECLARE_WINDOW_AGAIN_(name_win, width, height, scene, scene_var) \
  RENDERMETA_STRUCT_LIST_ RENDERMETA_WINDOW_ ## name_win ## _[scene]; \
  const int RENDERMETA_WINDOW_ ## name_win ## _WIDTH_  = width; \
  const int RENDERMETA_WINDOW_ ## name_win ## _HEIGHT_ = height; \
  void RENDERMETA_WINDOW_ ## name_win ## _INIT_(void) { \
    for (int i = 0; i < scene; i++) { \
      RENDERMETA_WINDOW_ ## name_win ## _[i].head = NULL; \
      RENDERMETA_WINDOW_ ## name_win ## _[i].head = NULL; \
    } \
  } \
  void RENDERMETA_WINDOW_ ## name_win ## _CB_DISPLAY_(void) { \
    RENDERMETA_STRUCT_LIST_E_ *RENDERMETA_E_ = RENDERMETA_WINDOW_ ## name_win ## _[scene_var].head; \
    glClear(GL_COLOR_BUFFER_BIT); \
    while (RENDERMETA_E_ != NULL) { \
      if (((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display != NULL) { \
        glPushMatrix(); \
          glTranslated(((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x, ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y, 0); \
          ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display(); \
        glPopMatrix(); \
      } \
      RENDERMETA_E_ = RENDERMETA_E_->next; \
    } \
    glFlush(); \
  } \
  void RENDERMETA_WINDOW_ ## name_win ## _CB_MOUSE_(int b, int s, int x, int y) { \
    RENDERMETA_STRUCT_LIST_E_ *RENDERMETA_E_ = RENDERMETA_WINDOW_ ## name_win ## _[scene_var].tail; \
    while (RENDERMETA_E_ != NULL) { \
      if ( \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_mouse != NULL \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x <= x && x <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.w \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y <= y && y <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.h \
      ) { \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_mouse(b, s, x - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x, y - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y); \
        if (((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display != NULL) { \
          break; \
        } \
      } \
      RENDERMETA_E_ = RENDERMETA_E_->prev; \
    } \
    glutPostRedisplay(); \
  } \
  void RENDERMETA_WINDOW_ ## name_win ## _CB_MOTION_(int x, int y) { \
    RENDERMETA_STRUCT_LIST_E_ *RENDERMETA_E_ = RENDERMETA_WINDOW_ ## name_win ## _[scene_var].tail; \
    while (RENDERMETA_E_ != NULL) { \
      if ( \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_motion != NULL \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x <= x && x <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.w \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y <= y && y <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.h \
      ) { \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_motion(x - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x, y - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y); \
        if (((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display != NULL) { \
          break; \
        } \
      } \
      RENDERMETA_E_ = RENDERMETA_E_->prev; \
    } \
    glutPostRedisplay(); \
  } \
  void RENDERMETA_WINDOW_ ## name_win ## _CB_PASSIVEMOTION_(int x, int y) { \
    RENDERMETA_STRUCT_LIST_E_ *RENDERMETA_E_ = RENDERMETA_WINDOW_ ## name_win ## _[scene_var].tail; \
    while (RENDERMETA_E_ != NULL) { \
      if ( \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_passiveMotion != NULL \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x <= x && x <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.w \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y <= y && y <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.h \
      ) { \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_passiveMotion(x - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x, y - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y); \
        if (((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display != NULL) { \
          break; \
        } \
      } \
      RENDERMETA_E_ = RENDERMETA_E_->prev; \
    } \
    glutPostRedisplay(); \
  } \
  void RENDERMETA_WINDOW_ ## name_win ## _CB_KEYBOARD_(unsigned char c, int x, int y) { \
    RENDERMETA_STRUCT_LIST_E_ *RENDERMETA_E_ = RENDERMETA_WINDOW_ ## name_win ## _[scene_var].tail; \
    while (RENDERMETA_E_ != NULL) { \
      if ( \
        ( \
          ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display == NULL\
          || ( \
            ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x <= x && x <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.w \
            && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y <= y && y <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.h \
          ) \
        ) \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_keyboard != NULL \
      ) { \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_keyboard(c, x - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x, y - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y); \
        if (((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display != NULL) { \
          break; \
        } \
      } \
      RENDERMETA_E_ = RENDERMETA_E_->prev; \
    } \
    glutPostRedisplay(); \
  } \
  void RENDERMETA_WINDOW_ ## name_win ## _CB_SPECIAL_(int c, int x, int y) { \
    RENDERMETA_STRUCT_LIST_E_ *RENDERMETA_E_ = RENDERMETA_WINDOW_ ## name_win ## _[scene_var].tail; \
    while (RENDERMETA_E_ != NULL) { \
      if ( \
        ( \
          ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display == NULL\
          || ( \
            ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x <= x && x <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.w \
            && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y <= y && y <= ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y + ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.h \
          ) \
        ) \
        && ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_special != NULL \
      ) { \
        ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_special(c, x - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x, y - ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y); \
        if (((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display != NULL) { \
          break; \
        } \
      } \
      RENDERMETA_E_ = RENDERMETA_E_->prev;\
    } \
    glutPostRedisplay(); \
  }
#define RENDER_DECLARE_WINDOW(name_win, width, height, scene, scene_var) RENDERMETA_DECLARE_WINDOW_AGAIN_(name_win, width, height, scene, scene_var)

#define RENDERMETA_DECLARE_OBJECT_AGAIN_(name_obj, width, height) \
  const int RENDERMETA_OBJECT_ ## name_obj ## _WIDTH_  = (width); \
  const int RENDERMETA_OBJECT_ ## name_obj ## _HEIGHT_ = (height);
#define RENDER_DECLARE_OBJECT(name_obj, width, height) RENDERMETA_DECLARE_OBJECT_AGAIN_(name_obj, width, height)

#define RENDER_DECLARE_OBJECT_FUNC_NULL(name_obj, name_func) void * const RENDER_ ## name_obj ## _ ## name_func = NULL

#define RENDERMETA_WINDOW_AGAIN_(name_win, title, posx, posy) \
  do { \
    RENDERMETA_WINDOW_ ## name_win ## _INIT_(); \
    glutInitWindowPosition(posx, posy); \
    glutInitWindowSize(RENDERMETA_WINDOW_ ## name_win ## _WIDTH_, RENDERMETA_WINDOW_ ## name_win ## _HEIGHT_); \
    glutCreateWindow(title); \
    glutDisplayFunc(RENDERMETA_WINDOW_ ## name_win ## _CB_DISPLAY_); \
    glutMouseFunc(RENDERMETA_WINDOW_ ## name_win ## _CB_MOUSE_); \
    glutMotionFunc(RENDERMETA_WINDOW_ ## name_win ## _CB_MOTION_); \
    glutPassiveMotionFunc(RENDERMETA_WINDOW_ ## name_win ## _CB_PASSIVEMOTION_); \
    glutKeyboardFunc(RENDERMETA_WINDOW_ ## name_win ## _CB_KEYBOARD_); \
    glutSpecialFunc(RENDERMETA_WINDOW_ ## name_win ## _CB_SPECIAL_); \
  } while (0)
#define RENDER_WINDOW(name_win, title, posx, posy) RENDERMETA_WINDOW_AGAIN_(name_win, title, posx, posy)

#define RENDERMETA_OBJECT_AGAIN_(name_win, scene, name_obj, posx, posy) \
  do { \
    RENDERMETA_STRUCT_LIST_E_ * const RENDERMETA_E_ = (RENDERMETA_STRUCT_LIST_E_ *)malloc(sizeof(RENDERMETA_STRUCT_LIST_E_)); \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.x = posx; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->position.y = posy; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.w = RENDERMETA_OBJECT_ ## name_obj ## _WIDTH_; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->size.h = RENDERMETA_OBJECT_ ## name_obj ## _HEIGHT_; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_display       = RENDER_ ## name_obj ## _display; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_mouse         = RENDER_ ## name_obj ## _mouse; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_motion = RENDER_ ## name_obj ## _motion; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_passiveMotion = RENDER_ ## name_obj ## _passiveMotion; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_keyboard      = RENDER_ ## name_obj ## _keyboard; \
    ((RENDERMETA_STRUCT_ *)RENDERMETA_E_)->func_special       = RENDER_ ## name_obj ## _special; \
    RENDERMETA_E_->next = NULL; \
    RENDERMETA_E_->prev = RENDERMETA_WINDOW_ ## name_win ## _[scene].tail; \
    if (RENDERMETA_WINDOW_ ## name_win ## _[scene].head == NULL) { \
      RENDERMETA_WINDOW_ ## name_win ## _[scene].head = RENDERMETA_E_; \
    } \
    else { \
      RENDERMETA_WINDOW_ ## name_win ## _[scene].tail->next = RENDERMETA_E_; \
    } \
    RENDERMETA_WINDOW_ ## name_win ## _[scene].tail = RENDERMETA_E_; \
  } while (0)
#define RENDER_OBJECT(name_win, scene, name_obj, posx, posy) RENDERMETA_OBJECT_AGAIN_(name_win, scene, name_obj, posx, posy)

#endif