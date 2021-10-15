
#ifndef _COMMANDKEY_H_
#define _COMMANDKEY_H_ 1

#include"Command.h"

/**
 *
 */
class PVDECLSPEC CommandKey : public Command {
public:

	CommandKey(unsigned int keycode, unsigned int mod, Scene *scene);
	CommandKey(const CommandKey &other);
	~CommandKey();

	/**
	 * Get keycode.
	 * @return
	 */
	const unsigned int getKeyCode() const;

	/**
	 * Get modification key.
	 * @return
	 */
	const unsigned int getModKey() const;

	/**
	 *
	 * @return
	 */
	virtual CommandType getType() const;

private:
	unsigned int keycode;    /*	*/
	unsigned int mod;        /*	*/
};

#endif