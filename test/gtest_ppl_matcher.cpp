/*!
  \file        gtest_ppl_matcher.cpp
  \author      Arnaud Ramey <arnaud.a.ramey@gmail.com>
                -- Robotics Lab, University Carlos III of Madrid
  \date        2014/2/1

________________________________________________________________________________

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
________________________________________________________________________________

Some tests for PPLMatcherTemplate.
 */
#include "people_recognition_vision/pplm_testing.h"
#include "vision_utils/filename_prefix2imgs.h"

class FooPPLMatcher : public PPLMatcherTemplate {
public:
  FooPPLMatcher() : PPLMatcherTemplate("FOO_PPLM_START", "FOO_PPLM_STOP") {
  }
  bool match(const PPL & new_ppl, const PPL & tracks, std::vector<double> & costs,
             std::vector<std::string> & new_ppl_added_tagnames,
                     std::vector<std::string> & new_ppl_added_tags,
                     std::vector<unsigned int> & new_ppl_added_indices,
             std::vector<std::string> & tracks_added_tagnames,
                     std::vector<std::string> & tracks_added_tags,
                     std::vector<unsigned int> & tracks_added_indices) {
    unsigned int ntracks = tracks.people.size(),
        ncurr_users = new_ppl.people.size();
    costs.resize(ntracks * ncurr_users, 1);
    // set diagonal costs to 0
    for (unsigned int curr_idx = 0; curr_idx < ncurr_users; ++curr_idx) {
      for (unsigned int track_idx = 0; track_idx < ntracks; ++track_idx) {
        if (track_idx != curr_idx)
          continue;
        int cost_idx = curr_idx * ntracks + track_idx;
        costs[cost_idx] = 0;
      } // end for (track_idx)
    } // end for (curr_idx)
    return true;
  }
private:
}; // end class FooPPLMatcher

////////////////////////////////////////////////////////////////////////////////

TEST(TestSuite, create) {
  if (!vision_utils::rosmaster_alive()) return;
  FooPPLMatcher matcher;
  matcher.start();
  matcher.stop();
}

////////////////////////////////////////////////////////////////////////////////

TEST(TestSuite, test_sizes) {
  if (!vision_utils::rosmaster_alive()) return;
  FooPPLMatcher matcher;
  vision_utils::test_sizes(matcher);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TestSuite, test_same_msg) {
  if (!vision_utils::rosmaster_alive()) return;
  FooPPLMatcher matcher;
  for (unsigned int nusers = 0; nusers < 10; ++nusers)
    vision_utils::test_same_msg(matcher, nusers);
}

////////////////////////////////////////////////////////////////////////////////

TEST(TestSuite, pplm_benchmark) {
  if (!vision_utils::rosmaster_alive()) return;
  FooPPLMatcher matcher;
  vision_utils::FilenamePrefix2Imgs db_player;
  ASSERT_TRUE(db_player.from_file(vision_utils::IMG_DIR() +  "breast/*_rgb.png"));
  vision_utils::pplm_benchmark(matcher, db_player, 1);
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){
  ros::init(argc, argv, "gtest_FooPPLMatcher");
  // Run all the tests that were declared with TEST()
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
