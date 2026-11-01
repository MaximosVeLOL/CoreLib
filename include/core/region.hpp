#ifndef __REGION_H__
#define __REGION_H__

/* Region
* A region is a fixed list meant to store data
* that can be added or removed
* 
*/

#include <core/types.hpp>
#include <core/libapi.hpp>
#include <stdexcept>


CORE_DECLARE_NAMESPACE

namespace FileSystem {
	struct CFile;
}


enum ERegionEvent : u8 {
	R_EVENT_ADD_START = 0,
	R_EVENT_ADD_LIMIT = 1,
	R_EVENT_ADD_END = 2,

	R_EVENT_DEL_START = 3,
	//Deletion methods
	//Do not delete
	R_EVENT_DEL_AS_SIMPLE,
	//delete m_Data[index]
	R_EVENT_DEL_AS_OBJECT,
	//delete[] m_Data[index]
	R_EVENT_DEL_AS_ARRAY,
	R_EVENT_DEL_END,

	R_EVENT_CLEAR,
	R_EVENT_COUNT,
};



template<typename Type, typename Count, Count Maximum>
class CRegionBase {
private:

protected:
	void _handleRemove(Count p_Index, bool p_PushOthers, u8 p_Amount) {
		m_Count -= p_Amount;
		if (!p_PushOthers) return;
		for (u8 i = p_Index;i < m_Count;i++) {
			m_Data[i] = m_Data[i + p_Amount];
		}
	}
public:

	Type* m_Data = nullptr;
	Count m_Count = 0;

	void Init() {
		m_Data = new Type[Maximum]{ Type() };
		if (!m_Data) {
			throw std::runtime_error("A region has failed to initialize");
		}
		//m_Count = 0;
	}

	virtual bool OnEvent(ERegionEvent p_Type, Type* p_Element) = 0;

	void Add(Type& p_Element) {
		OnEvent(R_EVENT_ADD_START, &p_Element);
		if (m_Count + 1 > Maximum) {
			OnEvent(R_EVENT_ADD_LIMIT, &p_Element);
		}
		m_Data[m_Count++] = p_Element;
		OnEvent(R_EVENT_ADD_END, &p_Element);
	}

	virtual void Clear() = 0;

	Type& GetAt(Count p_Index) {
		if (m_Count == 0) {
			throw std::runtime_error("A region's m_Count is 0!");
		}
		return static_cast<Type&>(this->m_Data[p_Index]);
	}

	Type& GetFirst() {
		return GetAt(0);
	}
	
	Type& GetLast() {
		return GetAt(this->m_Count - 1);
	}

	Type* begin() {
		return m_Data;
	}
	Type* end() {
		return m_Data + m_Count;
	}

	virtual void Import(FileSystem::CFile& p_File) = 0;
	virtual void Export(FileSystem::CFile& p_File) = 0;
};

//A simple region! Only use this when you have an array that has constant value. Basically, don't put pointers in here!
template <typename Type, typename Count, Count Maximum>
class CSimpleRegion : public CRegionBase<Type, Count, Maximum> {
protected:
	using CRegionBase<Type, Count, Maximum>::_handleRemove;
public:

	void Remove(Count p_Index = Maximum, bool p_PushOthers = true, u8 p_Amount = 1) {
		if (p_Index == Maximum) {
			p_Index = this->m_Count - 1;
		}
		this->_handleRemove(p_Index, p_PushOthers, p_Amount);
	}

	bool OnEvent(ERegionEvent p_Type, Type* p_Element) override = 0;
	void Clear() override = 0;
	void Import(FileSystem::CFile& p_File) override = 0;
	void Export(FileSystem::CFile& p_File) override = 0;

};
//A pointer region! Use this when you have arrays, or pointers to data in an array. So, GameObject*[], and other stuff like that.
template <typename Type, typename Count, Count Maximum>
class CPointerRegion : public CRegionBase<Type, Count, Maximum> {
protected:
	using CRegionBase<Type, Count, Maximum>::_handleRemove;
public:
	

	enum ERemoveType : u8 {
		R_DONT = 0,
		R_ASOBJECT = 1,
		R_ASARRAY = 2,
	};

	void Remove(ERemoveType p_Type = R_DONT, Count p_Index = 0xFF, bool p_PushOthers = true, u8 p_Amount = 1) {
		OnEvent(R_EVENT_DEL_START, nullptr);
		if (p_Index == Maximum) {
			p_Index = this->m_Count - 1;
		}
		switch (p_Type) {
		case R_DONT:
			OnEvent(R_EVENT_DEL_AS_SIMPLE, &this->m_Data[p_Index]);
			break;

		case R_ASOBJECT:
			OnEvent(R_EVENT_DEL_AS_OBJECT, &this->m_Data[p_Index]);
			delete this->m_Data[p_Index];
			break;

		case R_ASARRAY:
			OnEvent(R_EVENT_DEL_AS_ARRAY, &this->m_Data[p_Index]);
			delete[] this->m_Data[p_Index];
			break;
		}
		this->_handleRemove(p_Index, p_PushOthers, p_Amount);
		OnEvent(R_EVENT_DEL_END, nullptr);
	}

	bool OnEvent(ERegionEvent p_Type, Type* p_Element) override = 0;
	void Clear() override = 0;
	void Import(FileSystem::CFile& p_File) override = 0;
	void Export(FileSystem::CFile& p_File) override = 0;

};


CORE_END_NAMESPACE


#endif