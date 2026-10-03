#ifndef __REGION_H__
#define __REGION_H__

/* Region
* A region is a fixed list meant to store data
* that can be added or removed
* 
*/

#include <core/types.hpp>
#include <core/libapi.hpp>



CORE_DECLARE_NAMESPACE

namespace FileSystem {
	struct CFile;
}


template<typename Type, typename Count>
struct CRegion {
	Type* m_Data = nullptr;
	Count m_Count = 0;
	Count m_Max = 0;

	enum EEventType : u8 {
		E_ADD_ADDSTART = 0,
		E_ADD_LIMIT,
		E_ADD_ADDEND,
		E_DELETE_START,
		E_DELETE_ELEMENT_OBJECT,
		E_DELETE_ELEMENT_ARRAY,
		E_DELETE_END
	};

	virtual bool OnEvent(EEventType p_EEventType, Type* p_Element) {}

	void Add(Type p_Element) {
		AddEvent(E_ADD_ADDSTART, &p_Element);
		if (m_Count + 1 >= m_Max) {
			OnEvent(E_ADD_LIMIT, nullptr);
		}
		m_Data[m_Count++] = p_Element;
		OnEvent(E_ADD_ADDEND, &p_Element);
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
			OnEvent(E_DELETE_ELEMENT_OBJECT, m_Data[p_Index]);
			delete m_Data[p_Index];
			break;
		case DELETE_ARRAY:
			OnEvent(E_DELETE_ELEMENT_ARRAY, m_Data[p_Index]);
			delete[] m_Data[p_Index];

			break;
		}
		m_Count -= p_Amount;
		if (!p_PushOthers) return;
		for (u8 i = p_Index;i < m_Count;i++) {
			m_Data[i] = m_Data[i + p_Amount];
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

	virtual void Import(FileSystem::CFile& p_File) = 0;
	virtual void Export(FileSystem::CFile& p_File) = 0;

};


CORE_END_NAMESPACE


#endif