#include "../include/MonoImage.h"
#include "../include/MonoObject.h"
#include "../include/MonoMethod.h"
#include "../include/MonoLogger.h"

#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/mono-config.h>

namespace Mono
{
	ObjectPtr Image::CallStaticMethod(const char* signature, Args& args)
	{
		// Build a method description object
		auto methodDesc = Method::CreateDesc(signature, true);
		if (!methodDesc->IsValid())
		{
			return ObjectPtr();
		}

		// Search the method in the image
		MonoMethod* method = mono_method_desc_search_in_image(*methodDesc, m_typeInstance);
		if (!method)
		{
			return ObjectPtr();
		}

		// run the method
		MonoObject* ret = nullptr;

		// to capture any exception
		MonoObject* pException = nullptr;

		if (args.IsEmpty())
		{
			ret = mono_runtime_invoke(method, nullptr, nullptr, &pException);
		}
		else
		{
			ret = mono_runtime_invoke(method, nullptr, &args[0], &pException);
		}

		// halt method call on any exception
		if (pException)
		{
			Mono::GetLogger()->Exception(pException);
			return ObjectPtr();
		}

		if (ret)
		{
			return ObjectPtr(new Object(ret));
		}

		return ObjectPtr();
	}
}
