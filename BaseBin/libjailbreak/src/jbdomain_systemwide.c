#include "jbserver_xpc.h"

static bool systemwide_domain_allowed(audit_token_t clientToken)
{
	return true;
}

static int systemwide_get_jb_root(char **pathOut)
{
	*pathOut = strdup("/var/jb");
	return 0;
}

static int systemwide_get_boot_uuid(char **uuidOut)
{
	const char *launchdUUID = getenv("LAUNCHD_UUID");
	if (launchdUUID) {
		*uuidOut = strdup(launchdUUID);
	}
	else {
		*uuidOut = NULL;
	}
	return 0;
}

static int systemwide_trust_binary(char *binaryPath)
{
	return 0;
}

static int systemwide_process_checkin(audit_token_t *processToken)
{
	return 0;
}

static int systemwide_fork_fix(audit_token_t *parentToken, uint64_t childPid)
{
	return 0;
}

struct jbserver_domain gSystemwideDomain = {
	.permissionHandler = systemwide_domain_allowed,
	.actions = {
		// JBS_SYSTEMWIDE_GET_JB_ROOT
		{
			.handler = systemwide_get_jb_root,
			.args = (jbserver_arg[]){
				{ .name = "jb-path", .type = JBS_TYPE_STRING, .out = true },
				{ 0 },
			},
		},
		// JBS_SYSTEMWIDE_GET_BOOT_UUID
		{
			.handler = systemwide_get_boot_uuid,
			.args = (jbserver_arg[]){
				{ .name = "boot-uuid", .type = JBS_TYPE_STRING, .out = true },
				{ 0 },
			},
		},
		// JBS_SYSTEMWIDE_TRUST_BINARY
		{
			.handler = systemwide_trust_binary,
			.args = (jbserver_arg[]){
				{ .name = "binary-path", .type = JBS_TYPE_STRING, .out = false },
				{ 0 },
			},
		},
		// JBS_SYSTEMWIDE_PROCESS_CHECKIN
		{
			.handler = systemwide_process_checkin,
			.args = (jbserver_arg[]) {
				{ .name = "caller-token", .type = JBS_TYPE_CALLER_TOKEN, .out = false },
				{ 0 },
			},
		},
		// JBS_SYSTEMWIDE_FORK_FIX
		{
			.handler = systemwide_fork_fix,
			.args = (jbserver_arg[]) {
				{ .name = "caller-token", .type = JBS_TYPE_CALLER_TOKEN, .out = false },
				{ .name = "child-pid", .type = JBS_TYPE_UINT64, .out = false },
				{ 0 },
			},
		},
		{ 0 },
	},
};