#include "ui_variables.h"

scythe::ui::c_vars::c_vars()
{
}

scythe::ui::c_vars::~c_vars()
{
}

scythe::ui::c_vars& scythe::ui::c_vars::singleton()
{
	static c_vars p_object;
	return p_object;
}
