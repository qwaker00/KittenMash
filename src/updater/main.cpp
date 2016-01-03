#include <orm/vote.h>
#include <orm/rating.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

namespace {
    double calcEloAdd(double rA, double , double rB, double score) {
        const double K = rA < 2100 ? 32 : (rA < 2400 ? 24 : 16);
        const double eA = 1.0 / (1.0 + pow(10.0, (rB - rA) / 400));
        return K * 3 * (score - eA);
    }
}

int main() {
    Db db;
    while (true) {
        std::vector<DbVote> votes = DbVote::getPending(db, 10);
        if (votes.size() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        for (const auto& vote : votes) {
            std::cout << "Process vote #" << vote.getId() << ": ";
            if (vote.getResult() == EVoteResult::Left) {
                std::cout << vote.getLeftId() << " better than " << vote.getRightId();
            } else {
                std::cout << vote.getRightId() << " better than " << vote.getLeftId();
            }
            std::cout << "...";

            DbRating leftRating(db), rightRating(db);
            EVoteResult resultValue = vote.getResult();

            leftRating.getById(vote.getLeftId());
            rightRating.getById(vote.getRightId());
            leftRating.addToRating( calcEloAdd(leftRating.getRating(), leftRating.getGameCount(), rightRating.getRating(), resultValue == EVoteResult::Left) );
            rightRating.addToRating( calcEloAdd(rightRating.getRating(), rightRating.getGameCount(), leftRating.getRating(), resultValue == EVoteResult::Right) );
            vote.resetPending();

            std::cout << " Done." << std::endl;
         }
    }

    return 0;
}

