#pragma once

#include <hash_map>

#include "Foundation/DynamicArray.h"
#include "Reflect/Type.h"

namespace Helium
{
	namespace Reflect
	{
		//
		// Empty struct just for type deduction purposes
		//  don't worry though, even though this class is non-zero in size on its own,
		//  your derived struct type can use the memory this takes due to C/C++ standard
		//  'Empty Base Optimization'
		//
		struct HELIUM_REFLECT_API EnumerationBase
		{
		};

		class HELIUM_REFLECT_API EnumerationElement
		{
		public:
			EnumerationElement();
			EnumerationElement( uint32_t value, const tstring& name, const tstring& helpText = TXT( "FIXME: SET THE HELP TEXT FOR THIS ENUMERATION ELEMENT" ) );

			uint32_t    m_Value;    // the value of the object
			tstring     m_Name;     // the name of the object
			tstring     m_HelpText; // the help text for the object
		};

		class HELIUM_REFLECT_API Enumeration : public Type
		{
		public:
			REFLECTION_TYPE( ReflectionTypes::Enumeration, Enumeration, Type );

			Enumeration();
			~Enumeration();

			template<class T>
			static void Create( Enumeration const*& pointer, const tchar_t* name );

			virtual void Register() const HELIUM_OVERRIDE;
			virtual void Unregister() const HELIUM_OVERRIDE;

			void AddElement(uint32_t value, const tstring& name, const tstring& helpText = TXT( "FIXME: SET THE HELP TEXT FOR THIS ENUMERATION ELEMENT" ) );
			bool IsValid(uint32_t value) const;

			bool GetValue(const tstring& str, uint32_t& value) const;
			bool GetString(const uint32_t value, tstring& str) const;

			bool GetBitfieldValue(const std::vector< tstring >& strs, uint32_t& value) const;
			bool GetBitfieldStrings(const uint32_t value, std::vector< tstring >& strs) const;

			inline static bool IsFlagSet(uint32_t value, uint32_t flag);
			inline static void SetFlags(uint32_t& value, uint32_t flags);

			DynamicArray< EnumerationElement > m_Elements;
			bool                               m_IsBitfield;
		};

		template< class EnumT >
		class EnumRegistrar : public TypeRegistrar
		{
		public:
			EnumRegistrar(const tchar_t* name);
			~EnumRegistrar();

			virtual void Register();
			virtual void Unregister();
		};
	}
}

// declares type checking functions
#define _REFLECT_DECLARE_ENUMERATION( ENUMERATION ) \
public: \
Enum m_Value; \
ENUMERATION() : m_Value() {} \
ENUMERATION( const ENUMERATION& e ) : m_Value( e.m_Value ) {} \
ENUMERATION( const Enum& e ) : m_Value( e ) {} \
operator const Enum&() const { return m_Value; } \
static const Helium::Reflect::Enumeration* CreateEnumeration(); \
static const Helium::Reflect::Enumeration* s_Enumeration; \
static Helium::Reflect::EnumRegistrar< ENUMERATION > s_Registrar;

// defines the static type info vars
#define _REFLECT_DEFINE_ENUMERATION( ENUMERATION ) \
const Helium::Reflect::Enumeration* ENUMERATION::CreateEnumeration() \
{ \
	HELIUM_ASSERT( s_Enumeration == NULL ); \
	Reflect::Enumeration::Create< ENUMERATION >( s_Enumeration, TXT( #ENUMERATION ) ); \
	return s_Enumeration; \
} \
const Helium::Reflect::Enumeration* ENUMERATION::s_Enumeration = NULL; \
Helium::Reflect::EnumRegistrar< ENUMERATION > ENUMERATION::s_Registrar( TXT( #ENUMERATION ) );

// declares an enumeration
#define REFLECT_DECLARE_ENUMERATION( ENUMERATION ) \
	_REFLECT_DECLARE_ENUMERATION( ENUMERATION )

// defines an enumeration
#define REFLECT_DEFINE_ENUMERATION( ENUMERATION ) \
	_REFLECT_DEFINE_ENUMERATION( ENUMERATION )

#include "Reflect/Enumeration.inl"