/** @file
    @brief Implementation

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2015 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include "HDKLedIdentifier.h"
#include "IdentifierHelpers.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace vbtracker {

    OsvrHdkLedIdentifier::~OsvrHdkLedIdentifier() {}
    // Convert from string encoding representations into lists
    // of boolean values for use in comparison.
    OsvrHdkLedIdentifier::OsvrHdkLedIdentifier(
        const PatternStringList &PATTERNS) {
        // Ensure that we have at least one entry in our list and
        // find the length of the first entry.
        if (PATTERNS.size() == 0) {
            d_length = 0;
            return;
        }
        d_length = PATTERNS[0].size();

        // Decode each string into a new list of boolean values, making
        // sure each have the correct length.
        for (size_t i = 0; i < PATTERNS.size(); i++) {

            // Make sure the pattern is the correct length.
            if (PATTERNS[i].size() != d_length) {
                d_patterns.clear();
                d_length = 0;
                return;
            }

            // Make a wrapped pattern, which is the original pattern plus
            // a second copy of the pattern that has all but the last
            // character in it.  This will enable use to use the string
            // find() routine to see if any shift of the pattern is a
            // match.
            std::string wrapped = PATTERNS[i] + PATTERNS[i];
            wrapped.pop_back();

            // Add the pattern to the vector of lists.
            d_patterns.push_back(wrapped);
        }
        // std::cout << "XXX d_length = " << d_length << ", num patterns = " <<
        // d_patterns.size() << std::endl;
    }

    int OsvrHdkLedIdentifier::getId(std::list<float> &brightnesses, bool & lastBright) const {
        // If we don't have at least the required number of frames of data, we
        // don't know anything.
        if (brightnesses.size() < d_length) {
            return -1;
        }

        // We only care about the d_length most-recent levels.
        truncateBrightnessListTo(brightnesses, d_length);

        // Compute the minimum and maximum brightness values.  If
        // they are too close to each other, we have a light rather
        // than an LED.  If not, compute a threshold to separate the
        // 0's and 1's.
        Brightness minVal, maxVal;
        std::tie(minVal, maxVal) = findMinMaxBrightness(brightnesses);
        // Brightness is currently actually keypoint diameter in pixels, and
        // it's being under-estimated by OpenCV.
        static const double TODO_MIN_BRIGHTNESS_DIFF = 0.3;
        if (maxVal - minVal <= TODO_MIN_BRIGHTNESS_DIFF) {
            return -2;
        }
        const auto threshold = (minVal + maxVal) / 2;
        // Set the `lastBright` out variable
        lastBright = brightnesses.back() >= threshold;

        // Get a list of boolean values for 0's and 1's using
        // the threshold computed above.
        auto bits = getBitsUsingThreshold(brightnesses, threshold);

        // Search through the available patterns to see if the passed-in
        // pattern matches any of them.  If so, return that pattern.  We
        // need to check all potential rotations of the pattern, since we
        // don't know when the code started.  For the HDK, the codes are
        // rotationally invariant.  We do this by making wrapped strings
        // and seeing if the pattern shows up anywhe in them, relying on
        // the std::string find method to do efficiently.
        for (size_t i = 0; i < d_patterns.size(); i++) {
          if (d_patterns[i].find(bits) != std::string::npos) {
            return static_cast<int>(i);
          }
        }

        // No pattern recognized and we should have recognized one, so return
        // a low negative.  We've used -2 so return -3.
        return -3;
    }

} // End namespace vbtracker
} // End namespace osvr
