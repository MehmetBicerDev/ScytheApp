#pragma once
#include "net_constants.h"
#include <unordered_map>





namespace scythe
{
	class c_role
	{
	public:
		inline c_role() {}

		uint64_t id = 0;
		std::string name = "";
		uint32_t colour = 0xFFFFFFFF;

		void add_permission(net::permission permission);

		void remove_permission(net::permission permission);

		bool has_permission(net::permission permission);

		void clear_permissions();

		void set_administrator();
	private:
		std::unordered_map<net::permission, bool > permissions;
	};
}