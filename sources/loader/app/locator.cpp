#include <loader/app/locator.h>

using namespace loader;

std::unique_ptr<ResourceHolderNop<ImFont>> Locator::s_fonts_ = nullptr;



void Locator::Finalize()
{
}


ResourceHolderNop<ImFont>* Locator::GetFonts()
{
    if (!s_fonts_)
    {
        s_fonts_ = std::make_unique<ResourceHolderNop<ImFont>>();
    }

    return s_fonts_.get();
}
