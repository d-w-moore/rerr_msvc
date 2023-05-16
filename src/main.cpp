/// \file

#include "irods/irods_error.hpp"
#include "irods/irods_logger.hpp"
#include "irods/irods_ms_plugin.hpp"
#include "irods/irods_re_structs.hpp"
#include "irods/msParam.h"
#include "irods/rodsErrorTable.h"

#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <iterator>

namespace
{
    auto msi_rError_append(MsParam* _input_int_code, MsParam* _input_text, ruleExecInfo_t* _rei) -> int
    {
        using log_msi = irods::experimental::log::microservice;

        // Check input parameters.
        if (!_input_int_code || !_input_text || !_rei) { // NOLINT(readability-implicit-bool-conversion)
            log_msi::error("At least one input argument is null.");
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }

        int int_code = parseMspForPosInt(_input_int_code);

        const auto* text = parseMspForStr(_input_text);
        if (!text) { // NOLINT(readability-implicit-bool-conversion)
            log_msi::error("Could not parse microservice parameter into a string.");
            return SYS_INVALID_INPUT_PARAM;
        }

        return addRErrorMsg(&_rei->rsComm->rError, -abs(int_code), text);
    } // msi_impl

    template <typename... Args, typename Function>
    auto make_msi(const std::string& name, Function func) -> irods::ms_table_entry*
    {
        auto* msi = new irods::ms_table_entry{sizeof...(Args)}; // NOLINT(cppcoreguidelines-owning-memory) 
        msi->add_operation(name, std::function<int(Args..., ruleExecInfo_t*)>(func));
        return msi;
    } // make_msi
} // anonymous namespace

extern "C"
auto plugin_factory() -> irods::ms_table_entry*
{
    return make_msi<MsParam*, MsParam*>("msi_rError_append", msi_rError_append);
} // plugin_factory

#ifdef IRODS_FOR_DOXYGEN
/// Rotates a string.
///
/// \param[in]     _input_int_code  An integer holding the error code to be entered into the rError stack.
/// \param[in]     _input_message   A string containing the message to be entered into the rError stack.
/// \param[in,out] _rei    A ::RuleExecInfo object that is automatically handled by the
///                        rule engine plugin framework. Users must ignore this parameter.
///
/// \return An integer.
/// \retval 0        On success.
/// \retval Non-zero On failure.
///
/// \since X.Y.Z
int msi_rotate_string(MsParam* _input, MsParam* _output, ruleExecInfo_t* _rei);
#endif // IRODS_FOR_DOXYGEN
