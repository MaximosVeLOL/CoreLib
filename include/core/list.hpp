#ifndef __LIST_H__
#define __LIST_H__

#include <core/libapi.hpp>
#include <core/memory.hpp>
#include <limits>
#include <core/error.hpp>
#include <core/types.hpp>

#include <initializer_list>

CORE_DECLARE_NAMESPACE

#define CORE_LIST_USEOLD 1


#if CORE_LIST_USEOLD

template<typename Type, typename Count = unsigned __int16>
class CList {
private:

protected:
	Count _getMaxCount() {
		return std::numeric_limits<Count>::max();
	}

	Count m_Count = 0;
	Type* m_Data = nullptr;

	bool _checkNull() {
		if (!m_Data) {
			//throw("Implement");
			//m_Data = p_NewElements;
			//m_Count = p_ElementCount;
			return true;
		}
		return false;
	}
	bool _checkOutOfBounds(s64 p_Index) {
		return (p_Index < 0 || p_Index >= static_cast<s64>(m_Count));
	}


	virtual void _extend(unsigned __int8 p_ElementCount, Type* p_NewElements = nullptr) {
		if (static_cast<size_t>(static_cast<size_t>(m_Count) + p_ElementCount) >= _getMaxCount()) {
			throw("Exceeded type (Count) limit!");
		}
		if (_checkNull()) {
			m_Data = p_NewElements;
			m_Count = static_cast<Count>(p_ElementCount);
			return;
		}
		Type* output = new Type[m_Count + p_ElementCount];
		Count i = 0;
		for (; i < m_Count;i++) {
			output[i] = m_Data[i];
		}
		if (p_ElementCount == 1) {
			output[i] = *p_NewElements;
		}
		else {
			for (Count j = 0; j < p_ElementCount;j++) {
				output[i + j] = m_Data[j];
			}
		}
		if (m_Count == 1) {
			delete m_Data;
		}
		else delete[] m_Data;
		m_Data = output;
		m_Count += p_ElementCount;
	}

public:
	/*
	virtual void Push() {
		_extend(1, new Type());
	}
	*/
	virtual void push(Type p_Type) {
		//We can't use _extend due to it assuming everything is heap, so to the hacks we go!
		if (static_cast<size_t>(static_cast<size_t>(m_Count) + 1) >= _getMaxCount()) {
			throw("Exceeded type (Count) limit!");
		}
		if (_checkNull()) {
			m_Data = new Type[1]{ Type() };
			m_Data[0] = p_Type;
			m_Count = 1;
			return;
		}
		Type* output = new Type[m_Count + 1]{ Type() };
		Count i = 0;
		for (; i < m_Count;i++) {
			output[i] = m_Data[i];
		}
		output[i] = p_Type;
		m_Count++;
		delete[] m_Data;
		m_Data = output;
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
	void push_some(Count p_Count, Type* p_Array) {
		_extend(p_Count, p_Array);
	}
	void push_some(CList<Type, Count> p_Other) {
		_extend(p_Other.m_Count, p_Other.m_Data);
	}
	void erase(Count p_Index, Count p_Amount = 1) {
		Count newSize = (m_Count - p_Amount);
		Type* output = new Type[newSize];
		for (Count i = p_Index;i < newSize;i++) {
			output[i] = m_Data[i + p_Amount];
		}
		delete[] m_Data;
		m_Data = output;
		m_Count = newSize;
	}

	void pop() {
		erase(m_Count - 1, 1);
	}
	//A copy of length()
	Count size() {
		return m_Count;
	}
	Count length() {
		return m_Count;
	}
	bool is_empty() {
		return (static_cast<bool>(!m_Data));
	}

	//Returns a copy of the list's data
	Type* data() {
		Type* ret = new Type[m_Count]{ Type() };
		mem::Copy(m_Data, ret, m_Count * sizeof(Type));
		return ret;
	}
	//Returns a raw
	Type* data_raw() {
		return m_Data;
	}

	Type& at(Count p_Index) {
		return static_cast<Type&>(m_Data[p_Index]);
	}

	Type& operator[](Count p_Index) {
		return at(p_Index);
	}

	Type* at_safe(Count p_Index) {
		if (_checkOutOfBounds(p_Index)) {
			throw Error::CExOutOfBounds();
		}
		return &m_Data[p_Index];
	}
	Type& last() {
		return at(m_Count - 1);
	}
	Type& first() {
		return at(0);
	}

	Type* begin() {
		return m_Data;
	}
	Type* end() {
		return (m_Data + m_Count);
	}

	CList() {

	}
	CList(std::initializer_list<Type> p_List) {
		m_Data = new Type[p_List.size()];
		m_Count = p_List.size();
		for (Count i = 0; i < p_List.size();i++) {
			m_Data[i] = *static_cast<const Type*>(p_List.data() + (i * sizeof(Type)));
		}
		//throw std::runtime_error("Implement CList std initializer!");
	}
	CList(std::nullptr_t p_List) {
		//Set the data to invalid just incase...
		m_Data = nullptr;
		m_Count = 0;
	}

};

CORE_END_NAMESPACE

#else

#define CORE_LIST_USETYPES 0


template<typename Type, typename Length = u16, typename Increase = u8, typename Change = s8>
class CList {
private:
	void* m_Data = nullptr; //constant 8
	Length m_Length = 0; //ranges from 1 to 8 bytes
	//u8 m_Changes = 0;

public:

	Length size() {
		return m_Length;
	}
	Length length() {
		return m_Length;
	}

	void push(Type&& p_Element) {
		if (!m_Data) {
			//m_Data = mem::Allocate(sizeof(Type));
			Type* output = new Type[1]{ Type() };
			if (!output) {
				throw Error::CSystem();
			}
			output[0] = p_Element;
			//mem::Copy(&p_Element, m_Data, sizeof(Type));
			m_Data = output;
			m_Length = 1;
			return;
		}
		//void* output = mem::Allocate(sizeof(Type) * (m_Length + 1));
		m_Data = mem::Reallocate(m_Data, sizeof(Type) * (m_Length + 1));
		if (!m_Data)
			throw Error::CSystem();

		static_cast<Type*>(m_Data)[m_Length] = p_Element;
		//delete m_Data;
		m_Length++;
	}
	void push(Type& p_Element) {
		push(static_cast<Type&&>(p_Element));
	}


	Type& at(Length p_Index) {
		return static_cast<Type&>(data()[p_Index]);
	}
	Type& operator[](Length p_Index) {
		return at(p_Index);
	}
	Type& first() {
		return at(0);
	}
	Type& last() {
		return at(m_Length - 1);
	}

	Type* data() {
		return static_cast<Type*>(m_Data);
	}

	void push() {

	}

	Type* begin() {
		return data();
	}
	Type* end() {
		return (data() + m_Length);
	}

	bool operator==(Type*&& p_Other) {
		//If the lengths dont match
		if( (sizeof(p_Other) / sizeof(Type)) != m_Length)
			return false;
		//Compare each element
		for (Length i = 0; i < m_Length;i++) {
			if (data()[i] != p_Other[i])
				return false;
		}
		return true;
	}

	bool is_empty() {
		return static_cast<bool>(m_Data);
	}

};

CORE_END_NAMESPACE

#endif

#endif