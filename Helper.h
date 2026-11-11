#pragma once
namespace Helper
{
    std::ofstream CreateTimestampedFile(std::string* outPath = nullptr, const std::filesystem::path& directory = ".");
};

