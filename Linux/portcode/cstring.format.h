template<typename ParseContext>
constexpr auto fmt::formatter<CString>::parse(ParseContext& ctx)
{
  return ctx.begin();
}

template<typename FormatContext>
auto fmt::formatter<CString>::format(CString const& str, FormatContext& ctx)
{
  return fmt::format_to(ctx.out(), "%s", str.ToString());
}

template<typename... Args>
void CString::Format(const char* format, Args... args)
{
	m_data = fmt::sprintf(format, std::forward< Args >(args)...);
}
