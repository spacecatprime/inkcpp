#include "../include/MonoImage.h"
#include "../include/MonoObject.h"

#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/mono-config.h>

namespace Mono
{

	Image::Image()
	{
	}

	ObjectPtr Image::CallStaticMethod(const char* signature, Args& args)
	{
		// Build a method description object
		MonoMethodDesc* methodDesc = mono_method_desc_new(signature, true);
		if (!methodDesc)
		{
			return ObjectPtr();
		}

		// Search the method in the image
		MonoMethod* method = mono_method_desc_search_in_image(methodDesc, m_typeInstance);
		if (!method)
		{
			return ObjectPtr();
		}

		// run the method
		MonoObject* ret = nullptr;

		// TODO capture exception

		if (args.IsEmpty())
		{
			ret = mono_runtime_invoke(method, nullptr, nullptr, nullptr);
		}
		else
		{
			ret = mono_runtime_invoke(method, nullptr, &args[0], nullptr);
		}

		if (ret)
		{
			ObjectPtr out = ObjectPtr(new Object);
			out->SetInstance(ret);
			return out;
		}

		return ObjectPtr();
	}
}
