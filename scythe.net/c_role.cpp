#include "c_role.h"

void scythe::c_role::add_permission(net::permission permission)
{
    permissions[permission] = true;

}

void scythe::c_role::remove_permission(net::permission permission)
{
    permissions[permission] = false;
}

bool scythe::c_role::has_permission(net::permission permission)
{
    return permissions[permission];
}

void scythe::c_role::clear_permissions()
{
    for (auto permission : permissions)
        permissions[permission.first] = false;
}

void scythe::c_role::set_administrator()
{
    for (auto permission : permissions)
        permissions[permission.first] = true;
}
