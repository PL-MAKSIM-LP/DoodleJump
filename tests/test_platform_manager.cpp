#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

// Простой тест логики спавна без Framework

struct TestPlatform {
  float x, y;
  std::string type;
};

void testSpawnLogic() {
  std::cout << "\n=== PLATFORM SPAWN LOGIC TEST ===" << std::endl;

  int screenWidth = 800;
  int screenHeight = 600;
  const float JumpHeight = 120.0f;

  std::cout << "Screen: " << screenWidth << "x" << screenHeight << std::endl;

  // Логика computeSpawnGap() из PlatformManager
  auto computeGap = [screenHeight](float y) -> float {
    float easyThreshold = screenHeight - 50;
    float mediumThreshold = screenHeight - 150;

    if (y > easyThreshold) return 80.0f;     // LOW_GAP
    if (y > mediumThreshold) return 100.0f;  // MID_GAP
    return 150.0f;                           // HIGH_GAP
  };

  // Логика getDifficultyZone()
  auto getDifficulty = [screenHeight](float y) -> std::string {
    float easyThreshold = screenHeight - 50;
    float mediumThreshold = screenHeight - 150;
    float hardThreshold = screenHeight - 300;

    if (y > easyThreshold) return "EASY";
    if (y > mediumThreshold) return "MEDIUM";
    if (y > hardThreshold) return "HARD";
    return "EXTREME";
  };

  // Логика selectPlatformType() из PlatformManager
  auto getType = [screenHeight](float y, int index) -> std::string {
    float easyThreshold = screenHeight - 50;
    float mediumThreshold = screenHeight - 150;
    float hardThreshold = screenHeight - 300;

    if (y > easyThreshold) {
      return "NORMAL";  // EASY: только NORMAL
    }
    if (y > mediumThreshold) {
      // MEDIUM: 70% NORMAL, 30% MOVING_V
      return (index % 10 < 7) ? "NORMAL" : "MOVING_V";
    }
    if (y > hardThreshold) {
      // HARD: 40% MOVING_H, 30% MOVING_V, 30% BREAKABLE
      int type = index % 10;
      if (type < 4) return "MOVING_H";
      if (type < 7) return "MOVING_V";
      return "BREAKABLE";
    }
    // EXTREME: 25% MOVING_H, 25% MOVING_V, 25% BREAKABLE, 25% NORMAL
    int type = index % 4;
    if (type == 0) return "MOVING_H";
    if (type == 1) return "MOVING_V";
    if (type == 2) return "BREAKABLE";
    return "NORMAL";
  };

  // ===== TEST 1: BIG SPAWN =====
  std::cout << "\n--- TEST 1: FULL LEVEL (200 PLATFORMS) ---" << std::endl;

  float lastY = screenHeight - 50;  // Начальная позиция
  std::vector<TestPlatform> platforms;

  int normalCount = 0, movingHCount = 0, movingVCount = 0, breakableCount = 0;
  int easyCount = 0, mediumCount = 0, hardCount = 0, extremeCount = 0;

  std::cout << "Spawning 200 platforms..." << std::endl;

  for (int i = 0; i < 200; ++i) {
    // computeSpawnY() логика: gap + random 0.75-1.35
    float gap = computeGap(lastY);
    float gapVariance = 0.75f + (i % 4) * 0.2f;  // Симуляция рандома
    gap *= gapVariance;

    // Гарантируем минимальный зазор
    float minGap = JumpHeight + 10.0f;
    gap = std::max(gap, minGap);

    lastY -= gap;

    // Убедимся что платформа не ушла за экран
    if (lastY < 0) {
      break;
    }

    // computeSpawnX() логика
    float maxX = std::max(0.0f, static_cast<float>(screenWidth - 64));
    float spawnX = maxX * (0.3f + (i % 7) * 0.1f);  // Вариативность X

    std::string type = getType(lastY, i);
    TestPlatform p = {spawnX, lastY, type};
    platforms.push_back(p);

    // Статистика по типам
    if (type == "NORMAL")
      normalCount++;
    else if (type == "MOVING_H")
      movingHCount++;
    else if (type == "MOVING_V")
      movingVCount++;
    else if (type == "BREAKABLE")
      breakableCount++;

    // Статистика по зонам
    std::string zone = getDifficulty(lastY);
    if (zone == "EASY")
      easyCount++;
    else if (zone == "MEDIUM")
      mediumCount++;
    else if (zone == "HARD")
      hardCount++;
    else if (zone == "EXTREME")
      extremeCount++;
  }

  std::cout << "Total platforms spawned: " << platforms.size() << std::endl;

  // Проверка логики
  std::cout << "\n--- VALIDATION ---" << std::endl;

  float minY = platforms.back().y;
  float maxY = platforms[0].y;

  std::cout << "First platform Y: " << static_cast<int>(maxY) << std::endl;
  std::cout << "Last platform Y: " << static_cast<int>(minY) << std::endl;

  float verticalDistance = maxY - minY;
  std::cout << "Total vertical distance: " << static_cast<int>(verticalDistance)
            << std::endl;
  std::cout << "Avg gap: "
            << static_cast<int>(verticalDistance / platforms.size()) << "px"
            << std::endl;

  // Проверяем минимальный зазор
  bool gapViolation = false;
  float minRequiredGap = JumpHeight + 10.0f;

  for (size_t i = 1; i < platforms.size(); ++i) {
    float gap = platforms[i - 1].y - platforms[i].y;
    if (gap < minRequiredGap) {
      std::cout << "❌ Gap violation at platform " << i << ": " << std::fixed
                << std::setprecision(1) << gap << "px (min: " << minRequiredGap
                << "px)" << std::endl;
      gapViolation = true;
      break;
    }
  }

  if (!gapViolation) {
    std::cout << "✓ All gaps satisfy minimum distance requirement!"
              << std::endl;
  }

  // Статистика по типам платформ
  std::cout << "\n--- PLATFORM TYPE DISTRIBUTION ---" << std::endl;
  std::cout << "NORMAL:     " << std::setw(4) << normalCount << " ("
            << std::fixed << std::setprecision(1)
            << (normalCount * 100.0f / platforms.size()) << "%)" << std::endl;
  std::cout << "MOVING_H:   " << std::setw(4) << movingHCount << " ("
            << std::fixed << std::setprecision(1)
            << (movingHCount * 100.0f / platforms.size()) << "%)" << std::endl;
  std::cout << "MOVING_V:   " << std::setw(4) << movingVCount << " ("
            << std::fixed << std::setprecision(1)
            << (movingVCount * 100.0f / platforms.size()) << "%)" << std::endl;
  std::cout << "BREAKABLE:  " << std::setw(4) << breakableCount << " ("
            << std::fixed << std::setprecision(1)
            << (breakableCount * 100.0f / platforms.size()) << "%)"
            << std::endl;

  // Статистика по зонам сложности
  std::cout << "\n--- DIFFICULTY ZONE DISTRIBUTION ---" << std::endl;
  std::cout << "EASY:       " << std::setw(4) << easyCount << " (" << std::fixed
            << std::setprecision(1) << (easyCount * 100.0f / platforms.size())
            << "%)" << std::endl;
  std::cout << "MEDIUM:     " << std::setw(4) << mediumCount << " ("
            << std::fixed << std::setprecision(1)
            << (mediumCount * 100.0f / platforms.size()) << "%)" << std::endl;
  std::cout << "HARD:       " << std::setw(4) << hardCount << " (" << std::fixed
            << std::setprecision(1) << (hardCount * 100.0f / platforms.size())
            << "%)" << std::endl;
  std::cout << "EXTREME:    " << std::setw(4) << extremeCount << " ("
            << std::fixed << std::setprecision(1)
            << (extremeCount * 100.0f / platforms.size()) << "%)" << std::endl;

  // ===== TEST 2: SAMPLE PLATFORMS FROM EACH ZONE =====
  std::cout << "\n--- TEST 2: SAMPLE PLATFORMS ---" << std::endl;
  std::cout << std::setw(5) << "ID" << std::setw(10) << "X" << std::setw(10)
            << "Y" << std::setw(15) << "Difficulty" << std::setw(12) << "Type"
            << std::endl;
  std::cout << std::string(52, '-') << std::endl;

  // Выводим примеры из каждой зоны
  bool easyFound = false, mediumFound = false, hardFound = false,
       extremeFound = false;
  int sampleCount = 0;

  for (size_t i = 0; i < platforms.size() && sampleCount < 20; ++i) {
    const auto& p = platforms[i];
    std::string zone = getDifficulty(p.y);

    if ((zone == "EASY" && !easyFound) || (zone == "MEDIUM" && !mediumFound) ||
        (zone == "HARD" && !hardFound) ||
        (zone == "EXTREME" && !extremeFound)) {
      if (zone == "EASY") easyFound = true;
      if (zone == "MEDIUM") mediumFound = true;
      if (zone == "HARD") hardFound = true;
      if (zone == "EXTREME") extremeFound = true;

      std::cout << std::setw(5) << i << std::setw(10) << static_cast<int>(p.x)
                << std::setw(10) << static_cast<int>(p.y) << std::setw(15)
                << zone << std::setw(12) << p.type << std::endl;
      sampleCount++;
    }
  }

  // Также выводим последние несколько платформ (EXTREME зона)
  if (extremeFound) {
    std::cout << "...\n";
    for (size_t i = std::max(0UL, platforms.size() - 3); i < platforms.size();
         ++i) {
      const auto& p = platforms[i];
      std::cout << std::setw(5) << i << std::setw(10) << static_cast<int>(p.x)
                << std::setw(10) << static_cast<int>(p.y) << std::setw(15)
                << getDifficulty(p.y) << std::setw(12) << p.type << std::endl;
    }
  }

  std::cout << "\n=== TEST COMPLETE ===" << std::endl;
}

int main() {
  testSpawnLogic();
  return 0;
}
