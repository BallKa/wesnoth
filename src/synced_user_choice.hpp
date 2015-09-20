/*
   Copyright (C) 2015 by the Battle for Wesnoth Project

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#pragma once

#include "global.hpp"
#include "config.hpp"

#include <map>
#include <set>

namespace mp_sync
{

/**
 * Interface for querying local choices.
 * It has to support querying the user and making a random choice
 */
struct user_choice
{
	virtual ~user_choice() {}
	virtual config query_user(int side) const = 0;
	virtual config random_choice(int side) const = 0;
	///whether the choice is visible for the user like an advacement choice
	///a non-visible choice is for example get_global_variable
	virtual bool is_visible() const { return true; }
	virtual std::string description() const { return "input"; }
};

/**
 * Performs a choice for WML events.
 *
 * The choice is synchronized across all the multiplayer clients and
 * stored into the replay. The function object is called if the local
 * client is responsible for making the choice.
 * otherwise this function waits for a remote choice and returns it when it is received.
 * information about the choice made is saved in replay with dependent=true
 *
 * @param name Tag used for storing the choice into the replay.
 * @param side The number of the side responsible for making the choice.
 *             If zero, it defaults to the currently active side.
 *
 * @note In order to prevent issues with sync, crash, or infinite loop, a
 *       number of precautions must be taken when getting a choice from a
 *       specific side.
 *       - The server must recognize @name replay commands as legal from
 *         non-active players. Preferably the server should be notified
 *         about which player the data is expected from, and discard data
 *         from unexpected players.
 */
config get_user_choice(const std::string &name, const user_choice &uch,
	int side = 0);
/**
 * Performs a choice for mutiple sides for WML events.
 * uch is called on all sies specified in sides, this in done simulaniously on all those sides (or one after another if one client controlls mutiple sides)
 * and after all calls are executed the results are returned.
 */
std::map<int, config> get_user_choice_multiple_sides(const std::string &name, const user_choice &uch,
	std::set<int> sides);

}