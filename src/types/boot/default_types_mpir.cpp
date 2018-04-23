#include "types/common.h"

#include "default_types_mpir.h"

CRAFT_TYPE_DEFINE(mpz_class)
{
	_.add<GraphPropertyName>("gmplib.integer");
	_.add<GraphPropertyCppName>("mpz_class");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		try
		{
			return instance<mpz_class>::make(mpz_class(s));
		}
		catch (std::invalid_argument e)
		{
			throw stdext::exception("Cannot Parse Integer from:{0}", s);
		}
	});
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<mpz_class> _this) {
		return _this->get_str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(mpq_class)
{
	_.add<GraphPropertyName>("gmplib.rational");
	_.add<GraphPropertyCppName>("mpq_class");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		try
		{
			return instance<mpq_class>::make(mpz_class(s));
		}
		catch (std::invalid_argument e)
		{
			throw stdext::exception("Cannot Parse Rational from:{0}", s);
		}
	});
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<mpq_class> _this) {
		return _this->get_str();
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(mpf_class)
{
	_.add<GraphPropertyName>("gmplib.float");
	_.add<GraphPropertyCppName>("mpf_class");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		try
		{
			return instance<mpf_class>::make(mpf_class(s));
		}
		catch (std::invalid_argument e)
		{
			throw stdext::exception("Cannot Parse Float from:{0}", s);
		}
	});
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<mpf_class> _this) {
		mp_exp_t expptr;
		return fmt::format("{0}E{1}", _this->get_str(expptr), expptr);
	});
	_.use<PConstructor>().singleton<DefaultConstructor>();
}