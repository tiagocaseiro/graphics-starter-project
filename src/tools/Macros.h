#pragma

#define SHARED_ONLY(ClassName) using ClassName##Shared = std::shared_ptr<class ClassName>;