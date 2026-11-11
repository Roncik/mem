#include "pch.h"
#include "Helper.h"

std::ofstream Helper::CreateTimestampedFile(std::string* outPath, const std::filesystem::path& directory)
{
    // Generate timestamp
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
    localtime_s(&tm, &time);

    std::ostringstream filename;
    filename << std::put_time(&tm, "%Y.%m.%d-%H-%M-%S");

    // Generate random suffix
    static constexpr char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    static thread_local std::mt19937 rng{ std::random_device{}() };

    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    filename << '_';

    constexpr std::size_t suffixLength = 11;

    for (std::size_t i = 0; i < suffixLength; ++i)
        filename << charset[dist(rng)];

    filename << ".bin";

    // Create and open file
    auto path = (directory / filename.str());
    if (outPath)
        *outPath = path.string();

    return std::ofstream(path, std::ios::out | std::ios::binary);
}
