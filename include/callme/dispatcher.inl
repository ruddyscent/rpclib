
namespace callme {

template <typename F>
void dispatcher::bind(boost::string_ref name, F func,
                      detail::tags::void_result const &,
                      detail::tags::zero_arg const &) {
    funcs_.insert(
        std::make_pair(name, [func](msgpack::object const &args) {
            func();
            return std::make_unique<msgpack::object>();
        }));
}

template <typename F>
void dispatcher::bind(boost::string_ref name, F func,
                      detail::tags::void_result const &,
                      detail::tags::nonzero_arg const &) {
    using detail::func_traits;
    using args_type = typename func_traits<F>::args_type;

    funcs_.insert(
        std::make_pair(name, [func](msgpack::object const &args) {
            args_type args_real;
            args.convert(&args_real);
            detail::call(func, args_real);
            return std::make_unique<msgpack::object>();
        }));
}

template <typename F>
void dispatcher::bind(boost::string_ref name, F func,
                      detail::tags::nonvoid_result const &,
                      detail::tags::zero_arg const &) {
    using detail::func_traits;

    funcs_.insert(
        std::make_pair(name, [func](msgpack::object const &args) {
            return std::make_unique<msgpack::object>(func());
        }));
}

template <typename F>
void dispatcher::bind(boost::string_ref name, F func,
                      detail::tags::nonvoid_result const &,
                      detail::tags::nonzero_arg const &) {
    using detail::func_traits;
    using args_type = typename func_traits<F>::args_type;

    funcs_.insert(
        std::make_pair(name, [func](msgpack::object const &args) {
            args_type args_real;
            args.convert(&args_real);
            return std::make_unique<msgpack::object>(detail::call(func, args_real));
        }));
}
}