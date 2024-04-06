#ifndef MENU_COMMANDS_H
#define MENU_COMMANDS_H
// MI stands for Menu Item
#define MI_CYCLE_DRAW_MODE 10
#define MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES 21
#define MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET 22
#define MI_TOGGLE_DISPLAY_OF_BACKFACES 23
#define MI_TOGGLE_DISPLAY_OF_PICKING_RAY 24
#define MI_LOOKAT_CUBE 30
#define MI_RESET_CAMERA 40
#define MI_QUIT 50


#include "mjm_globals.h"

class menu_commands {
typedef menu_commands Myt;
//static Myt * r_one;
public:
static void menuCallback( int menuItem ) {
MM_ERR(MMPR2(__FUNCTION__,menuItem))
   switch ( menuItem ) {
      case MI_CYCLE_DRAW_MODE :
         ggb->drawMode = (glut_generic_base::DrawMode)( ( ggb->drawMode + 1 ) % ggb->NUM_DRAW_MODES );
         glutPostRedisplay();
         break;
      case MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES :
         ggb->drawWorldAxes = ! ggb->drawWorldAxes;
         glutPostRedisplay();
         break;
      case MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET :
//         drawTarget = ! drawTarget;
         glutPostRedisplay();
         break;
      case MI_TOGGLE_DISPLAY_OF_BACKFACES :
         ggb->cullBackfaces = ! ggb->cullBackfaces;
         glutPostRedisplay();
         break;
      case MI_TOGGLE_DISPLAY_OF_PICKING_RAY :
    //     ggb->drawPickingRay = ! ggb->drawPickingRay;
         glutPostRedisplay();
         break;
      case MI_LOOKAT_CUBE :
        // camera->lookAt( cubeCentre );
         glutPostRedisplay();
         break;
      case MI_RESET_CAMERA :
         r_cam.reset();
         glutPostRedisplay();
         break;
      case MI_QUIT :
         exit(0);
         break;
      default:
         printf("unknown menu item %d was selected\n", menuItem );
         break;
   }
}

static void right_menu()
{
// create a menu
MM_ERR(MMPR(__FUNCTION__))
   int mainMenu = glutCreateMenu( menuCallback );
   glutSetMenu( mainMenu ); // make it the current menu
   glutAddMenuEntry( "Cycle Display Mode (d)", MI_CYCLE_DRAW_MODE );
      int displayToggleMenu = glutCreateMenu( menuCallback );
      glutSetMenu( displayToggleMenu );
      glutAddMenuEntry( "World Space Axes (a)",
         MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES );
      glutAddMenuEntry( "Camera Target (t)",
         MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET );
      glutAddMenuEntry( "Backfaces (b)", MI_TOGGLE_DISPLAY_OF_BACKFACES );
      glutAddMenuEntry( "Picking Ray", MI_TOGGLE_DISPLAY_OF_PICKING_RAY );
   glutSetMenu( mainMenu );
   glutAddSubMenu( "Toggle Display of...", displayToggleMenu );
   glutAddMenuEntry( "Look At Cube (l)", MI_LOOKAT_CUBE );
   glutAddMenuEntry( "Reset Camera (r)", MI_RESET_CAMERA );
   glutAddMenuEntry( "Quit (Esc)", MI_QUIT );
   glutAttachMenu( GLUT_RIGHT_BUTTON );//attach the menu to the current window
   }
static void xkeyboardCallback( unsigned char key, int x, int y ) {
   switch ( key ) {
      case 'a':
         menuCallback( MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES );
         break;
      case 'b':
         menuCallback( MI_TOGGLE_DISPLAY_OF_BACKFACES );
         break;
      case 'd':
         menuCallback( MI_CYCLE_DRAW_MODE );
         break;
      case 'l':
         menuCallback( MI_LOOKAT_CUBE );
         break;
      case 'r':
         menuCallback( MI_RESET_CAMERA );
         break;
      case 't':
         menuCallback( MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET );
         break;
      case 27: // Escape
         menuCallback( MI_QUIT );
         break;
      default:
         printf("untrapped key %d\n", (int)key );
         break;
   }
}

};
#endif
