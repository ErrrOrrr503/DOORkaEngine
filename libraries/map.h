//
// Created by iduna on 3/13/2020.
//

#ifndef TLD_MAP_H
#define TLD_MAP_H

///@file map.h

namespace tld
{



#include "core/avl_yggdrasil.h"
	
	/**
	 * @brief A key-value pair used by map.
	 * @tparam KeyT Key type used in comparision and search.
	 * @tparam ValT Value type stored in pairs.
	 */
	template <typename KeyT, typename ValT>
	struct pair_t
	{
		KeyT _p_key;
		ValT _p_value;
		
		pair_t();
		
		explicit
		pair_t(const KeyT& nkey);
		
		pair_t(const KeyT& nkey, const ValT& nvalue);
		
		~pair_t();
		
		pair_t(const pair_t&);
	};

//template <typename KeyT, typename ValT>
//inline bool operator>(const pair_t<KeyT, ValT>& lht, const pair_t<KeyT, ValT>& rht);
	
	template <typename KeyT, typename ValT>
	inline bool operator <(const pair_t<KeyT, ValT>& lht, const pair_t<KeyT, ValT>& rht);
	
	template <typename KeyT, typename ValT>
	inline bool operator ==(const pair_t<KeyT, ValT>& lht, const pair_t<KeyT, ValT>& rht);
	
	/**
	 * @brief A data associative container type made made up of key-value pairs and providing data obtaining in logarithmic time.
	 *
	 * @tparam KeyT Key type (argument) of a pair.
	 * @tparam ValT	Data type (value) of a pair.
	 *
	 * @details This container is based on an AVL binary search tree algorithm. Key type in the pair must have overloaded comparision operators.
	 */
	template <typename KeyT, typename ValT>
	class map_t
	{
	public:
		typedef typename tld::avl_tree<pair_t<KeyT, ValT> >::tree_node node_t;
	
	private:
		tld::avl_tree<pair_t<KeyT, ValT> > m_tree_;
		
		node_t* Find(const KeyT& key, node_t* p_node);
	
	
	public:
		map_t();
		
		map_t(const map_t& that);
		
		~map_t();
		
		void Insert(const pair_t<KeyT, ValT>& _x);
		
		void Erase(const KeyT& key);
		
		ValT& Find(const KeyT& key);
		
		void Clear();
		
		bool Empty();
		
		ValT& operator [](const KeyT& key);
		
		map_t& operator =(const map_t& that);
	};
	
	/**
	 * @brief Default constructor. Calls default constructors of key type and value type.
	 */
	template <typename KeyT, typename ValT>
	pair_t<KeyT, ValT>::pair_t() : _p_key(), _p_value()
	{}
	
	/**
	 * @brief Crates a pair with default value and a key provided.
	 * @param nkey New key.
	 */
	template <typename KeyT, typename ValT>
	pair_t<KeyT, ValT>::pair_t(const KeyT& nkey) : _p_key(nkey), _p_value()
	{}
	
	/**
	 * @brief Creates a pair with a key and a value provided.
	 * @param nkey New key.
	 * @param nvalue New value.
	 */
	template <typename KeyT, typename ValT>
	pair_t<KeyT, ValT>::pair_t(const KeyT& nkey, const ValT& nvalue) : _p_key(nkey), _p_value(nvalue)
	{}
	
	/**
	 * @brief Copy constructor for a node class.
	 * @param that Reference to the source object.
	 */
	template <typename KeyT, typename ValT>
	pair_t<KeyT, ValT>::pair_t(const pair_t& that)
	{
		this->_p_key = that._p_key;
		this->_p_value = that._p_value;
	}
	
	
	template <typename KeyT, typename ValT>
	pair_t<KeyT, ValT>::~pair_t() = default;
	
	template <typename KeyT, typename ValT>
	map_t<KeyT, ValT>::~map_t() = default;
	
	/**
	 * @brief Creates an empty map with an empty tree.
	 */
	template <typename KeyT, typename ValT>
	map_t<KeyT, ValT>::map_t() = default;
	
	/**
	 * @brief Retrieves a value by a key.
	 *
	 * @param key
	 * @return Reference to the value component of the pair with provided key.
	 *
	 * @details If there is no pair with such key in the map, a new pair with
	 * such key and default value is created. Reference to this pair's value
	 * is returned.
	 */
	template <typename KeyT, typename ValT>
	ValT& map_t<KeyT, ValT>::Find(const KeyT& key)
	{
		node_t* res_node = Find(key, m_tree_.root_);
		
		if (res_node)
			return res_node->_n_value._p_value;
		
		pair_t<KeyT, ValT> n_node(key);
		return (m_tree_.Insert(n_node))->_n_value._p_value;
	}
	
	template <typename KeyT, typename ValT>
	typename map_t<KeyT, ValT>::node_t* map_t<KeyT, ValT>::Find(const KeyT& key, map_t::node_t* p_node)
	{
		if (p_node == nullptr)
			return nullptr;
		
		if (key == p_node->_n_value._p_key)
			return p_node;
		
		if (key < p_node->_n_value._p_key)
			return Find(key, p_node->left);
		
		return Find(key, p_node->right);
	}
	
	/**
	 * @brief An access operator. Retrieves a value by a key.
	 * @param key
	 * @return Reference to the value component of the pair with provided key.
	 *
	 * @details If there is no pair with such key in the map, a new pair with
	 * such key and default value is created. Reference to this pair's value
	 * is returned.
	 */
	template <typename KeyT, typename ValT>
	ValT& map_t<KeyT, ValT>::operator [](const KeyT& key)
	{
		node_t* res_node = Find(key, m_tree_.root_);
		
		if (res_node)
			return res_node->_n_value._p_value;
		
		pair_t<KeyT, ValT> n_node(key);
		return (m_tree_.Insert(n_node))->_n_value._p_value;
	}
	
	/**
	 * @brief Assignment-by-copy operator.
	 *
	 * @param that Reference to the source object.
	 * @return Reference to the map object to create assignment chains.
	 *
	 * @details Recursively copies pairs stored in the source object to
	 * the destination object.
	 */
	template <typename KeyT, typename ValT>
	map_t<KeyT, ValT>& map_t<KeyT, ValT>::operator =(const map_t& that)
	{
		if (this != &that)
		{
			this->m_tree_ = that.m_tree_;
		}
		return *this;
	}
	
	/**
	 * @brief Copy constructor for a map object.
	 *
	 * @param that Reference to the source object.
	 *
	 * @details Recursively copies pairs stored in the source object to
	 * the destination object.
	 */
	template <typename KeyT, typename ValT>
	map_t<KeyT, ValT>::map_t(const map_t& that)
	{
		this->m_tree_ = that.m_tree_;
	}
	
	/**
	 * @brief Removes the pair with provided key from the map.
	 * @param key
	 */
	template <typename KeyT, typename ValT>
	void map_t<KeyT, ValT>::Erase(const KeyT& key)
	{
		m_tree_.FindAndRemove(key);
	}
	
	/**
	 * @brief Empties map object by freeing recursively allocated memory.
	 */
	template <typename KeyT, typename ValT>
	void map_t<KeyT, ValT>::Clear()
	{
		m_tree_.Destroy();
	}
	
	/**
	 * @brief Says whether map object is empty or not.
	 * @return True if empty, false if not.
	 */
	template <typename KeyT, typename ValT>
	bool map_t<KeyT, ValT>::Empty()
	{
		return (m_tree_.root_ == nullptr);
	}
	
	/**
	 * @brief Inserts pair of suitable type into a map.
	 * @param _x Reference to the pair object.
	 */
	template <typename KeyT, typename ValT>
	void map_t<KeyT, ValT>::Insert(const pair_t<KeyT, ValT>& _x)
	{
		m_tree_.Insert(_x);
	}

/*template <typename KeyT, typename ValT>
inline bool operator>(const pair_t<KeyT, ValT>& lht, const pair_t<KeyT, ValT>& rht)
{
    return (lht.key > rht.key);
}*/
	/**
	 * @brief Comparision operator for pairs.
	 * @param lht Left operand.
	 * @param rht Right operand.
	 * @return The result of comparision between keys of the operands.
	 */
	template <typename KeyT, typename ValT>
	inline bool operator <(const pair_t<KeyT, ValT>& lht, const pair_t<KeyT, ValT>& rht)
	{
		return (lht._p_key < rht._p_key);
	}
	
	/**
	 * @brief Comparision operator for pairs.
	 * @param lht Left operand.
	 * @param rht Right operand.
	 * @return The result of comparision between keys of the operands.
	 */
	template <typename KeyT, typename ValT>
	inline bool operator ==(const pair_t<KeyT, ValT>& lht, const pair_t<KeyT, ValT>& rht)
	{
		return (lht._p_key == rht._p_key);
	}
	
	/**
	 * @brief Comparision operator for map type.
	 *
	 * @param lht Left operand.
	 * @param rht Right operand.
	 * @return The result of comparision of the trees.
	 *
	 * @details It is used to avoid self-assignment.
	 */
	template <typename KeyT, typename ValT>
	inline bool operator ==(const map_t<KeyT, ValT>& lht, const map_t<KeyT, ValT>& rht)
	{
		return (lht.m_tree_ == rht.m_tree_);
	}
	
}// namespace tld

#endif //TLD_MAP_H
