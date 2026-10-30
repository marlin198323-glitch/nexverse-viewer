/* Mozilla Public License 2.0
 * Optional OS protected Secret Service credential storage.
 * No passwords on disk as config, no stdout/stderr with secret material.
 */
#pragma once
#include <string>
#include <string_view>
#include <optional>
namespace nexverse::ui
{
struct SavedOglLogin
{
    std::string avatar;
    std::string password;
};
[[nodiscard]] bool login_keyring_supported() noexcept;
[[nodiscard]] std::optional<SavedOglLogin> load_saved_ogl_login();
[[nodiscard]] bool save_ogl_login(std::string_view avatar,
                                   std::string_view password);
[[nodiscard]] bool clear_saved_ogl_login();
}
