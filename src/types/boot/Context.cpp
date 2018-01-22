#include "../common.h"
#include "./Context.h"

using namespace craft;
using namespace craft::types;
using namespace craft::types::_details;

CRAFT_OBJECT_DEFINE(craft::types::Context)
{

	_.defaults();
}

/******************************************************************************
** _ContextQueryable
******************************************************************************/

enum class SearchKind
{
	Name,
	Type,
	Feature
};

class _details::_ContextQueryable
	: public std::enable_shared_from_this<_ContextQueryable>
	, public IContextQueryable
{
public:

private:
	inline std::shared_ptr<Context const> getRoot() const
	{
		if (_parentIsRoot)
		{
			return _parentRoot;
		}
		else if (_parent != nullptr)
		{
			return _parent->getRoot();
		}
		return nullptr;
	}

	_ContextQueryable(std::shared_ptr<_ContextQueryable const> parent)
		: _parentIsRoot(false)
		, _parent(parent)
		, _resolved(false)
	{
		_parentRoot = getRoot();
	}
	_ContextQueryable(std::shared_ptr<Context const> parent)
		: _parentIsRoot(true)
		, _parentRoot(parent)
		, _resolved(false)
	{
	}

public:
	_ContextQueryable(std::shared_ptr<_ContextQueryable const> parent, std::string value)
		: _ContextQueryable(parent)
	{
		_searchKind = SearchKind::Name;
		_searchName = value;
		auto it = _parentRoot->_primesByName.find(value);
		if (it != _parentRoot->_primesByName.end())
			_prime = it->second;
	}

	_ContextQueryable(std::shared_ptr<_ContextQueryable const> parent, SearchKind kind, size_t value)
		: _ContextQueryable(parent)
	{
		_searchKind = kind;
		if (_searchKind == SearchKind::Type)
		{
			_searchType = value;
			auto it = _parentRoot->_primesByType.find(value);
			if (it != _parentRoot->_primesByType.end())
				_prime = it->second;
		}
		if (_searchKind == SearchKind::Feature)
		{
			_searchFeature = value;
			auto it = _parentRoot->_primesByFeature.find(value);
			if (it != _parentRoot->_primesByFeature.end())
				_prime = it->second;
		}
	}

	_ContextQueryable(std::shared_ptr<Context const> parent, std::string value)
		: _ContextQueryable(parent)
	{
		_searchKind = SearchKind::Name;
		_searchName = value;
		auto it = _parentRoot->_primesByName.find(value);
		if (it != _parentRoot->_primesByName.end())
			_prime = it->second;
	}

	_ContextQueryable(std::shared_ptr<Context const> parent, SearchKind kind, size_t value)
		: _ContextQueryable(parent)
	{
		_searchKind = kind;
		if (_searchKind == SearchKind::Type)
		{
			_searchType = value;

			while (parent)
			{
				auto it = parent->_primesByType.find(value);
				if (it != parent->_primesByType.end())
				{
					_prime = it->second;
					break;
				}
				parent = parent->_parent;
			}
		}
		if (_searchKind == SearchKind::Feature)
		{
			_searchFeature = value;

			while (parent)
			{
				auto it = parent->_primesByFeature.find(value);
				if (it != parent->_primesByFeature.end())
				{
					_prime = it->second;
					break;
				}
				parent = parent->_parent;
			}
		}
	}

public:
	bool _parentIsRoot;
	std::shared_ptr<_ContextQueryable const> _parent;
	std::shared_ptr<Context const> _parentRoot;

	SearchKind _searchKind;
	std::string _searchName;
	TypeId _searchType;
	FeatureId _searchFeature;

	mutable instance<> _prime;
	mutable std::set<instance<>> _cache;

	mutable bool _resolved;

	void resolve() const
	{
		if (_resolved)
			return;

		Context* _cheat = ((Context*)_parentRoot.get());
		std::set<instance<>> const& set(
			(_searchKind == SearchKind::Name)
			? _cheat->_objectsByName[_searchName]
			: ((_searchKind == SearchKind::Type)
				? _cheat->_objectsByType[_searchType]
				: _cheat->_objectsByFeature[_searchFeature])
		);

		if (!_parentIsRoot)
		{
			_parent->resolve();

			std::set_union(
				_parent->_cache.begin(), _parent->_cache.end(),
				set.begin(), set.end(),
				std::inserter(_cache, _cache.end()));
		}
		else
		{
			_cache = set;
		}

		if (_cache.size() == 1)
			_prime = *_cache.begin();

		_resolved = true;
	}

	virtual std::shared_ptr<IContextQueryable> byName(std::string const& name) const override
	{
		return std::make_shared<_ContextQueryable>(shared_from_this(), name);
	}
	virtual std::shared_ptr<IContextQueryable> byFeature(FeatureId const& i_id) const override
	{
		return std::make_shared<_ContextQueryable>(shared_from_this(), SearchKind::Feature, (size_t)i_id);
	}
	virtual std::shared_ptr<IContextQueryable> byType(TypeId const& t_id) const override
	{
		return std::make_shared<_ContextQueryable>(shared_from_this(), SearchKind::Type, (size_t)t_id);
	}

	virtual std::set<instance<>> objects() const override
	{
		resolve();
		return _cache;
	}

	virtual instance<> prime() const override
	{
		resolve();
		return _prime;
	}
};

/******************************************************************************
** Context
******************************************************************************/

Context::Context(instance<> prime)
{
	_tree_depth = 0;
	_prime = prime;
	_finalized = false;
}

std::shared_ptr<Context> Context::makeChildContext(std::shared_ptr<Context> parent, instance<> prime)
{

	if (!parent)
	{
		throw stdext::exception("Parent must be defined in this static constructor");
	}

	std::shared_ptr<Context> res = std::make_shared<Context>(prime);
	res->_parent = parent;
	res->_tree_depth = parent->_tree_depth + 1;
	parent->_children.push_back(res);

	return res;
}


std::shared_ptr<Context> Context::makeMentorContext(std::shared_ptr<Context> mentor, instance<> prime)
{

	if (!mentor)
	{
		throw stdext::exception("Parent must be defined in this static constructor");
	}
	if (!mentor->finalized())
	{
		throw stdext::exception("Mentor must be finalized");
	}

	std::shared_ptr<Context> res = std::make_shared<Context>(prime);
	res->_parent = mentor;
	res->_tree_depth = 1;

	return res;
}

std::shared_ptr<Context> Context::copy() const
{
	auto res = std::make_shared<Context>(_prime);
	res->_parent = this->_parent;

	res->_all = this->_all;

	res->_objectsByType = this->_objectsByType;
	res->_objectsByFeature = this->_objectsByFeature;
	res->_objectsByName = this->_objectsByName;

	res->_primesByType = this->_primesByType;
	res->_primesByFeature = this->_primesByFeature;
	res->_primesByName = this->_primesByName;

	return res;
}
std::shared_ptr<Context> Context::parent() const
{
	return this->_parent;
}

void Context::addOnName(std::string const& name, instance<> obj)
{
	if (finalized()) throw stdext::exception("Context is finalized, cannot add.");

	_objectsByName[name].insert(obj);
	_all.insert(obj);
}

void Context::addOnType(TypeId t_id, instance<> obj)
{
	if (finalized()) throw stdext::exception("Context is finalized, cannot add.");

	_objectsByType[t_id].insert(obj);
	_all.insert(obj);
}

void Context::addOnFeature(FeatureId i_id, instance<> obj)
{
	if (finalized()) throw stdext::exception("Context is finalized, cannot add.");

	_objectsByFeature[i_id].insert(obj);
	_all.insert(obj);
}

void Context::promoteOnName(std::string const& name, instance<> obj)
{
	if (finalized()) throw stdext::exception("Context is finalized, cannot promote.");

	_primesByName[name] = obj;
	addOnName(name, obj);
}

void Context::promoteOnType(TypeId t_id, instance<> obj)
{
	if (finalized()) throw stdext::exception("Context is finalized, cannot promote.");

	_primesByType[t_id] = obj;
	addOnType(t_id, obj);
}

void Context::promoteOnFeature(FeatureId i_id, instance<> obj)
{
	if (finalized()) throw stdext::exception("Context is finalized, cannot promote.");

	_primesByFeature[i_id] = obj;
	addOnFeature(i_id, obj);
}

void Context::finalize()
{
	if (_parent && !_parent->finalized())
		throw stdext::exception("Parent context not finalized.");

	_finalized = true;
}
bool Context::finalized() const
{
	return _finalized;
}

std::shared_ptr<IContextQueryable> Context::byName(std::string const& name) const
{
	return std::make_shared<_ContextQueryable>(shared_from_this(), name);
}
std::shared_ptr<IContextQueryable> Context::byFeature(FeatureId const& i_id) const
{
	return std::make_shared<_ContextQueryable>(shared_from_this(), SearchKind::Feature, (size_t)i_id);
}
std::shared_ptr<IContextQueryable> Context::byType(TypeId const& t_id) const
{
	return std::make_shared<_ContextQueryable>(shared_from_this(), SearchKind::Type, (size_t)t_id);
}

std::set<instance<>> Context::objects() const
{
	return _all;
}

instance<> Context::prime() const
{
	return _prime;
}

void Context::recurse_expand(std::map<instance<>, bool>& expanded)
{
	
	if (_prime && (expanded.find(_prime) == expanded.end()))
	{
		_prime.getFeature<PObjectContextual>()->expand(_prime, shared_from_this());
		expanded[_prime] = true;
	}
	for (auto a : _all)
	{
		if (expanded.find(a) == expanded.end())
		{
			a.getFeature<PObjectContextual>()->expand(a, shared_from_this());
			expanded[a] = true;
		}
	}

	for (auto c : _children)
	{
		c->recurse_expand(expanded);
	}
}

void Context::recurse_contextualize()
{
	if (_prime)
	{
		_prime.getFeature<PObjectContextual>()->contextualize(_prime, shared_from_this());
	}

	for (auto a : _all)
	{
		a.getFeature<PObjectContextual>()->contextualize(a, shared_from_this());
	}

	finalize();

	for (auto c : _children)
	{
		c->recurse_contextualize();
	}
}

void Context::fishtacos()
{
	std::map<instance<>, bool> expanded;
	recurse_expand(expanded);
	recurse_contextualize();

	// Recurse
	

	
}
