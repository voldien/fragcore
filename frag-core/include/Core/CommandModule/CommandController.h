/**
    Simple physic simulation with a server-client model support.
    Copyright (C) 2017  Valdemar Lindberg

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
#ifndef _PV_COMMAND_CONTROLLER_H_
#define _PV_COMMAND_CONTROLLER_H_ 1
#include"Command.h"
#include"input/KeyMap.h"
#include"Queue.h"

/**
 * Responsible for redirect the command
 * to function call.
 */
class PVDECLSPEC CommandController {
public :

	CommandController();
	CommandController(const CommandController &other);
	~CommandController();

	/**
	 * 
	 * @param mask
	 */
	void setCommandMask(Command::CommandType mask);

	/**
	 * Add command to command controller.
	 * @param command
	 */
	void enqueue(const Command &command);

	/**
	 * Execute the command in front of the queue.
	 */
	void dequeue();

	/**
	 * Set keymapper used for mapping
	 * key input to function.
	 * @param keymapper
	 */
	void setKeyMap(KeyMap *keymapper);

protected:

	/**
	 *
	 * @param command
	 * @return
	 */
	bool isCommandMasked(const Command& command) const;

private:

	Queue<Command> queue;    /*	Rotation queue for rotating the command.	*/
	KeyMap *keymapper;        /*	*/
	Command::CommandType mask;

public:

	/**
	 *
	 * @param pResources
	 * @return
	 */
	static CommandController *createCommandController(Resources *pResources);
};

#endif
