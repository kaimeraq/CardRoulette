#include <Game/GameInstance.h>

static size_t s_allocationCount = 0;
static size_t s_deallocationCount = 0;

void* operator new(size_t size)
{
    ++s_allocationCount;
    return malloc(size);
}

void* operator new[](size_t size)
{
    ++s_allocationCount;
    return malloc(size);
}

void operator delete(void* ptr) noexcept
{
    ++s_deallocationCount;
    free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    ++s_deallocationCount;
    free(ptr);
}

void operator delete(void* ptr, size_t) noexcept
{
    ++s_deallocationCount;
    free(ptr);
}

void operator delete[](void* ptr, size_t) noexcept
{
    ++s_deallocationCount;
    free(ptr);
}

// Renderer baseline allocations = 0
// Single card render allocations = 12
// Hand render allocations = 12
// Deck render allocations = 12
// Printing render allocations = 10 per
int main()
{
    GameInstance instance;
    instance.Run();
}