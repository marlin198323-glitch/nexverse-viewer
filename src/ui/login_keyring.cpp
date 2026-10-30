#include "nexverse/ui/login_keyring.hpp"
#include <string_view>
#include <algorithm>
#if defined(NEXVERSE_HAVE_LIBSECRET)
#include <libsecret/secret.h>
#endif
namespace nexverse::ui
{
#if defined(NEXVERSE_HAVE_LIBSECRET)
namespace
{
const SecretSchema* schema() noexcept
{
    static const SecretSchema s={
        "de.nexvortex.opengenesislink.ogl-login",SECRET_SCHEMA_NONE,
        {{"grid",SECRET_SCHEMA_ATTRIBUTE_STRING},
         {"kind",SECRET_SCHEMA_ATTRIBUTE_STRING},
         {nullptr,static_cast<SecretSchemaAttributeType>(0)}}};
    return &s;
}
constexpr const char* grid="stadt-nexverse";
}
#endif
bool login_keyring_supported() noexcept
{
#if defined(NEXVERSE_HAVE_LIBSECRET)
    return true;
#else
    return false;
#endif
}
std::optional<SavedOglLogin> load_saved_ogl_login()
{
#if defined(NEXVERSE_HAVE_LIBSECRET)
    GError* error=nullptr;
    gchar* avatar=secret_password_lookup_sync(
        schema(),nullptr,&error,"grid",grid,"kind","avatar",nullptr);
    if(error!=nullptr){g_error_free(error);return std::nullopt;}
    if(avatar==nullptr)return std::nullopt;
    SavedOglLogin saved;
    saved.avatar=avatar;
    secret_password_free(avatar);
    if(saved.avatar.empty()||saved.avatar.size()>127U)return std::nullopt;
    gchar* password=secret_password_lookup_sync(
        schema(),nullptr,&error,"grid",grid,"kind","password",nullptr);
    if(error!=nullptr){g_error_free(error);return std::nullopt;}
    if(password==nullptr)return std::nullopt;
    saved.password=password;
    secret_password_free(password);
    if(saved.password.empty())return std::nullopt;
    return saved;
#else
    return std::nullopt;
#endif
}
bool save_ogl_login(std::string_view avatar,std::string_view password)
{
#if defined(NEXVERSE_HAVE_LIBSECRET)
    if(avatar.empty()||avatar.size()>127U||password.empty()||
       password.size()>4096U)return false;
    const std::string a{avatar};
    std::string p{password};
    GError* error=nullptr;
    const gboolean pw_ok=secret_password_store_sync(
        schema(),SECRET_COLLECTION_DEFAULT,
        "OpenGenesisLINK Viewer – NexVerse Passwort",p.c_str(),
        nullptr,&error,"grid",grid,"kind","password",nullptr);
    std::fill(p.begin(),p.end(),'\0');
    if(error){g_error_free(error);return false;}
    if(!pw_ok)return false;
    const gboolean name_ok=secret_password_store_sync(
        schema(),SECRET_COLLECTION_DEFAULT,
        "OpenGenesisLINK Viewer – NexVerse Avatarname",a.c_str(),
        nullptr,&error,"grid",grid,"kind","avatar",nullptr);
    if(error)
    {
        g_error_free(error);
        (void)secret_password_clear_sync(
            schema(),nullptr,nullptr,"grid",grid,"kind","password",nullptr);
        return false;
    }
    if(!name_ok)
    {
        (void)secret_password_clear_sync(
            schema(),nullptr,nullptr,"grid",grid,"kind","password",nullptr);
        return false;
    }
    return true;
#else
    (void)avatar;(void)password;
    return false;
#endif
}
bool clear_saved_ogl_login()
{
#if defined(NEXVERSE_HAVE_LIBSECRET)
    GError* error=nullptr;
    (void)secret_password_clear_sync(
        schema(),nullptr,&error,"grid",grid,"kind","avatar",nullptr);
    if(error){g_error_free(error);return false;}
    (void)secret_password_clear_sync(
        schema(),nullptr,&error,"grid",grid,"kind","password",nullptr);
    if(error){g_error_free(error);return false;}
    return true;
#else
    return false;
#endif
}
}
