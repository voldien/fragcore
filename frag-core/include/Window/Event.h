/**
	ELT (Engine Library Toolkit) is a cross platform engine toolkit
	Copyright (C) 2014  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#ifndef _ELT_EVENT_H_
#define _ELT_EVENT_H_ 1
#include "../elt_def.h"
#include "../ExNT.h"

#ifdef __cplusplus /*	C++ Environment	*/
extern "C"
{
#endif

	/**
 *
 */
	typedef struct ex_point_t
	{
		int x, y; /*	*/
	} ExPoint;

	/**
 *
 */
	typedef struct ex_size_t
	{
		unsigned int width;	 /*	*/
		unsigned int height; /*	*/
	} ExSize;

	/**
 *
 */
	typedef struct ex_rect_t
	{
		int x;		/*	*/
		int y;		/*	*/
		int width;	/*	*/
		int height; /*	*/
	} ExRect;

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

	typedef struct ex_system_event_t
	{
		unsigned int message; /**/
	} ExSystemEvent;

	typedef struct ex_joy_stick_event_t
	{

		unsigned char button[5]; /**/
	} ExJoyStickEvent;

	typedef struct ex_joystick_move_event_t
	{

		/*
	 *
	 */
		unsigned int x[3];
	} ExJoySticMoveEvent;

	typedef struct ex_joystick_button_event_t
	{

		/*
	 *
	 */
		unsigned char button[8];
	} ExJoySticButtonEvent;

	typedef struct ex_mouse_move_event_t
	{
		int x; /*	*/
		int y; /*	*/
	} ExMouseMoveEvent;

	typedef struct ex_mouse_motion_event_t
	{
		int x;		/**/
		int y;		/**/
		int xdelta; /**/
		int ydelta; /**/
	} ExMouseMotionEvent;

	typedef struct elt_win_button_event_t
	{
		unsigned char button; /**/
	} ExWinButtonEvent;

	typedef struct ex_mouse_wheel_event_t
	{
		int delta; /**/
		int x, y;  /**/
	} ExMouseWheelEvent;

	typedef struct ex_key_event_t
	{
		unsigned char code;
		unsigned char alt;
		unsigned char shift;
		unsigned char system;
		unsigned char ctrl;
	} ExKeyEvent;

	typedef struct ex_drop_event_t
	{
		/**/
		int number;
		/**/
		int cize;
	} ExDropEvent;

	typedef struct ex_touch_finger_event_t
	{
		unsigned int type;	   /*              */
		unsigned int touchid;  /*              */
		unsigned int fingerid; /*              */
		float x;			   /*              */
		float y;			   /*              */
		float dx;			   /*              */
		float dy;			   /*              */
		float pressure;		   /*              */
	} ExTouchFingerEvent;

	typedef struct ex_window_destroy_t
	{
		ExWin window; /*	*/
	} ExEventDestroyedWindow;

	typedef struct ex_window_reparent_t
	{
		ExWin parent; /*	*/
		ExWin child;  /*	*/
	} ExWindowReparent;

	typedef struct window_poll_events_t
	{
		unsigned int event;				   /*      */
		ExKeyEvent key;					   /*      */
		ExSize size;					   /*      */
		ExMouseMoveEvent mouse;			   /*      */
		ExMouseWheelEvent mouseWheelEvent; /*      */
		ExWinButtonEvent button;		   /*      */
		ExDropEvent drop;				   /*      */
		unsigned long int time;			   /*		*/
		ExDisplay display;				   /*		*/
		ExWin window;
		ExWindowReparent reparent;
	} ExWindowEvent;

	/**
 *
 */
	typedef struct elt_poll_events_t
	{
		unsigned int event;				   /*	event type fetched.	*/
		ExKeyEvent key;					   /*	key event info.		*/
		ExMouseMoveEvent mouse;			   /*          */
		ExMouseWheelEvent mouseWheelEvent; /*          */
		ExWinButtonEvent button;		   /*          */
		ExSystemEvent sys;				   /*          */
		ExDropEvent drop;				   /*          */
		ExSize size;					   /*          */
		ExSystemEvent system;			   /*          */
		ExTouchFingerEvent touch;		   /*          */
		ExMouseMotionEvent motion;		   /*			*/
		ExEventDestroyedWindow destroy;	   /*			*/
		unsigned long int time;			   /*			*/
		ExDisplay display;				   /*			*/
		/*ExPoint location;	*/
		ExWindowReparent reparent; /*	*/
		ExWin window;			   /*	*/
	} ExEvent;

	/**
 *	Poll event from process.
 *
 *	\event non null pointer to ExEvent event structure.
 *	if event is NULL, there will be a segmentation violation.
 *
 *	@Return 1 if event was polled. 0 if no event was polled.
 */
	extern ELTDECLSPEC int ELTAPIENTRY ExPollEvent(ExEvent *event);

	/**
 *	Poll Window Event information
 *
 *	\window which window to poll event information.
 *	Remark: message feed will only be apply to given HWND paramter
 *	if HWND is null all window create on this application will be update
 *	read more at MSDN for more specific information regarding PeekMessage with null HWND.
 *
 *	\event pointer to event struct. all event will be stored in it.
 *
 *	@return
 */
	extern ELTDECLSPEC int ELTAPIENTRY ExPollWindowEvent(ExWin window,
														 ExWindowEvent *event);

	/**
 *
 *	@Return
 */
	extern ELTDECLSPEC int ELTAPIENTRY ExForwardEvent(unsigned int event,
													  ExHandle data, unsigned int size);

#ifdef __cplusplus /*	C++ Environment	*/
}
#endif
#endif
