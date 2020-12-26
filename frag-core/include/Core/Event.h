/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2018  Valdemar Lindberg

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
#ifndef _FRAG_CORE_EVENT_H_
#define _FRAG_CORE_EVENT_H_ 1
//TODO improve for adding event based model for the engine

///**
// *	Event base class.
// */
//class VDDECLSPEC VDEvent{
//		public:
//
//		VDEvent(void);
//		VDEvent(const VDEvent& event);
//		virtual ~VDEvent(void);
//
//		/**
//		 *	On resize event.
//		 */
//		virtual void VDAPIENTRY onResize(int width, int height);
//
//		/**
//		 *	on mouse move or clicks a button.
//		 */
//		virtual void VDAPIENTRY onMouse(int x, int y, int button);
//
//		/**
//		 *	on mouse motion.
//		 */
//		virtual void VDAPIENTRY onMouseMotion(int xmotion, int ymotion, VDInput::Button button);
//
//		/**
//		 *	On mouse pressed event.
//		 */
//		virtual void VDAPIENTRY onMousePressed(VDInput::Button pressed);
//
//		/**
//		 *	On mouse button release.
//		 */
//		virtual void VDAPIENTRY onMouseReleased(VDInput::Button released);
//
//		/**
//		 *	On mouse wheel event.
//		 */
//		virtual void VDAPIENTRY onMouseWheel(int direction);
//
//		/**
//		 *	On key pressed event.
//		 */
//		virtual void VDAPIENTRY onKeyPressed(Uint32 keycode);
//
//		/**
//		 *	On key release event.
//		 */
//		virtual void VDAPIENTRY onKeyRelease(Uint32 keycode);
//
//		/**
//		 *	On key event.
//		 */
//		virtual void VDAPIENTRY onKey(Uint32 keycode);
//
//
//		public:	/*	Static methods.	*/
//
//		/**
//		 *	Get number of registered event handler.
//		 */
//		static unsigned int VDAPIENTRY getNumEvents(void);
//
//		/**
//		 *	Get list of all event handlers.
//		 */
//		static VDVector<VDEvent*>* VDAPIENTRY getEvents(void);
//
//		/**
//		 *	Get event by index.
//		 */
//		static VDEvent* VDAPIENTRY getEvent(unsigned int index);
//};

#endif //FRAGVIEW_EVENT_H
