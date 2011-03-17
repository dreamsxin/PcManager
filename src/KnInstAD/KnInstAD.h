// KnInstAD.h

#include "resource.h"

class CKnInstADModule : public CAppModule
{
public:
    HRESULT Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL);

protected:
};

extern CKnInstADModule _Module;