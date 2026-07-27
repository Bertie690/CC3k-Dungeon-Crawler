export module featureflags;

#pragma once

// Singleton struct holding feature flags set during initialization.
export struct FeatureFlags {
  inline static bool useImprovedHoardResolution = false;
  inline static bool enableDebugMode = false;
  inline static bool enableEnhancedGraphics = false;
  inline static bool enableNumpadInput = false;
};
