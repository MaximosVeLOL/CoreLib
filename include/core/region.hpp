#ifndef __REGION_H__
#define __REGION_H__

/* Region
* A region is a dynamic array,
* With a count, a max value, and an array of type
* Includes custom stuff for importing and exporting
* I needed this for my projects, so here it is here!
*/

#include <core/types.hpp>
#include <core/libapi.hpp>



CORE_DECLARE_NAMESPACE

namespace FileSystem {
	struct File;
}


template<typename Type, typename Count>
struct Region {
	Type* m_Data = nullptr;
	Count m_Count = 0;
	Count m_Max = 0;

	enum EventType : u8 {
		E_ADD_ADDSTART = 0,
		E_ADD_LIMIT,
		E_ADD_ADDEND,
	};

	virtual bool AddEvent(EventType p_EventType, Type* p_Element) {}

	void Add(Type p_Element) {
		AddEvent(E_ADD_ADDSTART, &p_Element);
		if (m_Count + 1 >= m_Max) {
			AddEvent(E_ADD_LIMIT);
		}
		m_Data[m_Count++] = p_Element;
		AddEvent(E_ADD_ADDEND, &p_Element);
	}

	enum DeleteType : u8 {
		DELETE_DONT = 0,
		DELETE_OBJECT,
		DELETE_ARRAY,
	};

	void Remove(DeleteType p_DeleteType = DELETE_DONT, Count p_Index = 0xFF, bool p_PushOthers = true, u8 p_Amount = 1) {
		if (p_Index == 0xFF) {
			p_Index = m_Count - 1;
		}
		switch (p_DeleteType) {
		case DELETE_DONT:
			break;
		case DELETE_OBJECT:
			delete m_Data[p_Index];
			break;
		case DELETE_ARRAY:
			delete[] m_Data[p_Index];
			break;
		}
		if (!p_PushOthers) return;
		for (u8 i = 0 p_Index;i < m_Count;i++) {
			m_Data[i] = m_Data[i + p_Index];
		}
	}

	void Flush(DeleteType p_Type) {
		switch (p_DeleteType) {
		case DELETE_DONT:
			break;
		case DELETE_OBJECT:
			delete m_Data;
			break;
		case DELETE_ARRAY:
			delete[] m_Data;
			break;
		}
	}

	virtual void Import(FileSystem::File& p_File) = 0;
	virtual void Export(FileSystem::File& p_File) = 0;

};


CORE_END_NAMESPACE


#endif