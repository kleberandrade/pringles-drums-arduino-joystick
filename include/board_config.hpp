#ifndef PRINGLES_BOARD_CONFIG_HPP
#define PRINGLES_BOARD_CONFIG_HPP

namespace board {
constexpr uint8_t PadPins[] = {A2, A3};
constexpr char Keys[] = {'w', 'd'};
constexpr uint8_t ArmPin = 4;
constexpr uint32_t SampleMs = 1;
constexpr uint32_t ReportMs = 20;
constexpr size_t PadCount = sizeof(PadPins) / sizeof(PadPins[0]);
static_assert(PadCount == sizeof(Keys) / sizeof(Keys[0]), "One key per pad");
} // namespace board
#endif
