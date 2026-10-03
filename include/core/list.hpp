#ifndef __LIST_H__
#define __LIST_H__

#include <core/libapi.hpp>

CORE_DECLARE_NAMESPACE

template<typename Type, typename Count>
class CORE_API CList {
private:

protected:
	Count m_Count = 0;
	Type* m_Data = nullptr;

	bool _checkNull() {
		if (!m_Data) {
			throw("Implement");
			//m_Data = p_NewElements;
			//m_Count = p_ElementCount;
			return true;
		}
		return false;
	}

	void _extend(unsigned __int8 p_ElementCount, Type* p_NewElements = nullptr) {
		if (_checkNull()) {
			m_Data = p_NewElements;
			m_Count = static_cast<Count>(p_ElementCount);
		}
		Type* output = new Type[m_Count + p_ElementCount];
		Count i = 0;
		for (; i < m_Count;i++) {
			output[i] = m_Data[i];
		}
		if(p_ElementCount == 1) {
			output[i] = *p_NewElements;
		}
		else {
			for (Count j = 0; j < p_ElementCount;j++) {
				output[i + j] = m_Data[j];
			}
		}
		delete[] m_Data;
		m_Data = output;
		m_Count += p_ElementCount;
	}

public:
	virtual void Push(Type p_Type) {
		_extend(1, &p_Type);
	}
	/*
	template<typename... Args>
	void PushSome(Args... p_Elements) {
		if (typeid...(p_Elements) != typeid(Type)) {
			throw("The type is not correct!");
		}
		Count argSize = (sizeof...(p_Elements) / sizeof(Type));
		Type elements[argSize] = p_Elements...;
		_extend(argSize, elements);
	}
	*/
	void PushSome(Count p_Count, Type* p_Array) {
		_extend(p_Count, p_Array);
	}
	void Erase(Count p_Index, Count p_Amount = 1) {
		Count newSize = (m_Count - p_Amount);
		Type* output = new Type[newSize];
		for (Count i = p_Index;i < newSize;i++) {
			output[i] = m_Data[i + p_Amount];
		}
		delete[] m_Data;
		m_Data = output;
		m_Count = newSize;
	}

	void Pop() {
		Erase(m_Count - 1, 1);
	}

	Count GetSize() {
		return m_Count;
	}
	Type* GetData() {
		return m_Data;
	}

	Type& GetAt(Count p_Index) {
		return m_Data[p_Index];
	}
	Type& GetLast() {
		return m_Data[m_Count];
	}
	Type& GetFirst() {
		return m_Data[0];
	}
};

CORE_END_NAMESPACE

#endif