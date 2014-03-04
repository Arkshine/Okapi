
#include <hl_type_conversion.h>
#include <globals.h>

entvars_t* HL_TypeConversion::cbase_to_edict(void* cbase)
{
	if(!cbase)
		return NULL;

	return *(entvars_t **)((char *)(cbase) + G_OffsetHandler->PEV);
}
	