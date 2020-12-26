
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
	~CommandKey(void);

	/**
	 * Get keycode.
	 * @return
	 */
	const unsigned int getKeyCode(void) const;

	/**
	 * Get modification key.
	 * @return
	 */
	const unsigned int getModKey(void) const;

	/**
	 *
	 * @return
	 */
	virtual CommandType getType(void) const;

private:
	unsigned int keycode;    /*	*/
	unsigned int mod;        /*	*/
};

#endif