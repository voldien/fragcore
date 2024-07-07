/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAGCORE_EVENT_H_
#define _FRAGCORE_EVENT_H_ 1
#include "Window.h"
#include <FragCore.h>

namespace fragcore {

	/**
	 *
	 */
	using Point = struct point_t {
		int x, y; /*	*/
	};

	/**
	 *
	 */
	using Size = struct size_t {
		unsigned int width;	 /*	*/
		unsigned int height; /*	*/
	};

	/**
	 *
	 */
	using Rect = struct rect_t {
		int x;		/*	*/
		int y;		/*	*/
		int width;	/*	*/
		int height; /*	*/
	};

/**
 *	Event flag.
 */
#define EX_EVENT_KEY 0x1				  /*	Any key at any state.	*/
#define EX_EVENT_KEY_RELEASE 0x2		  /*	Any key in release state.	*/
#define EX_EVENT_KEY_PRESSED 0x4		  /*	Any key in pressed state.	*/
#define EX_EVENT_MOUSE 0x8				  /*	*/
#define EX_EVENT_MOUSE_PRESSED 0x10		  /*	On any mouse button pressed.	*/
#define EX_EVENT_MOUSE_RELEASED 0x20	  /*	On any mouse button released.	*/
#define EX_EVENT_SIZE 0x40				  /*	On window size change.	*/
#define EX_EVENT_RESIZE 0x40			  /*	TODO fix, does not really work.	*/
#define EX_EVENT_SYSTEM 0x80			  /*	On system event.	*/
#define EX_EVENT_MOUSEWHEEL 0x100		  /*	*/
#define EX_EVENT_JOYSTICK 0x200			  /*	*/
#define EX_EVENT_TOUCH 0x400			  /*	*/
#define EX_EVENT_DROP 0x800				  /*	*/
#define EX_EVENT_QUIT 0x1000			  /*	*/
#define EX_EVENT_MOUSE_MOTION 0x2000	  /*	*/
#define EX_EVENT_EXPOSE 0x4000			  /*	*/
#define EX_EVENT_ON_FOCUSE 0x8000		  /*	On window in focus.	*/
#define EX_EVENT_ON_UNFOCUSE 0x10000	  /*	On window unfocus.	*/
#define EX_EVENT_WINDOW_MOVE 0x20000	  /*	*/
#define EX_EVENT_WINDOW_DESTROYED 0x40000 /*	*/
#define EX_EVENT_WINDOW_REPARENT 0x80000  /*	*/
#define EX_EVENT_WINDOW_HIDE 0x100000	  /*	*/
#define EX_EVENT_WINDOW_SHOW 0x200000	  /*	*/

	using SystemEvent = struct system_event_t {
		unsigned int message; /**/
	};

	using JoyStickEvent = struct joy_stick_event_t {
		unsigned char button[5]; /**/
	};

	using JoySticMoveEvent = struct joystick_move_event_t {

		/*
		 *
		 */
		unsigned int x[3];
	};

	using JoySticButtonEvent = struct joystick_button_event_t {

		/*
		 *
		 */
		unsigned char button[8];
	};

	using MouseMoveEvent = struct mouse_move_event_t {
		int x; /*	*/
		int y; /*	*/
	};

	using MouseMotionEvent = struct mouse_motion_event_t {
		int x;		/**/
		int y;		/**/
		int xdelta; /**/
		int ydelta; /**/
	};

	using WinButtonEvent = struct win_button_event_t {
		unsigned char button; /**/
	};

	using MouseWheelEvent = struct mouse_wheel_event_t {
		int delta; /**/
		int x, y;  /**/
	};

	using KeyEvent = struct key_event_t {
		unsigned char code;
		unsigned char alt;
		unsigned char shift;
		unsigned char system;
		unsigned char ctrl;
	};

	using DropEvent = struct drop_event_t {
		/**/
		int number;
		/**/
		int cize;
	};

	using TouchFingerEvent = struct touch_finger_event_t {
		unsigned int type;	   /*              */
		unsigned int touchid;  /*              */
		unsigned int fingerid; /*              */
		float x;			   /*              */
		float y;			   /*              */
		float dx;			   /*              */
		float dy;			   /*              */
		float pressure;		   /*              */
	};

	using EventDestroyedWindow = struct window_destroy_t {
		Window *window; /*	*/
	};

	using WindowReparent = struct window_reparent_t {
		Window *parent; /*	*/
		Window *child;	/*	*/
	};

	using WindowEvent = struct window_poll_events_t {
		unsigned int event;				 /*      */
		KeyEvent key;					 /*      */
		Size size;						 /*      */
		MouseMoveEvent mouse;			 /*      */
		MouseWheelEvent mouseWheelEvent; /*      */
		WinButtonEvent button;			 /*      */
		DropEvent drop;					 /*      */
		unsigned long int time;			 /*		*/
		Display *display;				 /*		*/
		Window *window;
		WindowReparent reparent;
	};

	/**
	 *
	 */
	using Event = struct poll_events_t {
		unsigned int event;				 /*	event type fetched.	*/
		KeyEvent key;					 /*	key event info.		*/
		MouseMoveEvent mouse;			 /*          */
		MouseWheelEvent mouseWheelEvent; /*          */
		WinButtonEvent button;			 /*          */
		SystemEvent sys;				 /*          */
		DropEvent drop;					 /*          */
		Size size;						 /*          */
		SystemEvent system;				 /*          */
		TouchFingerEvent touch;			 /*          */
		MouseMotionEvent motion;		 /*			*/
		EventDestroyedWindow destroy;	 /*			*/
		unsigned long int time;			 /*			*/
		Display *display;				 /*			*/
		/*Point location;	*/
		WindowReparent reparent; /*	*/
		Window *window;			 /*	*/
	};
} // namespace fragcore

#endif
