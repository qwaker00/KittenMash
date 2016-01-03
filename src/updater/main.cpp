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
    size_t batchSize = 10;
    while (true) {
        std::vector<DbVote> votes = DbVote::getPending(db, batchSize);

        if (votes.size() == 0) {
            if (batchSize <= 10) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } else {
                batchSize /= 2;
            }
        } else {
            if (votes.size() == batchSize) {
                batchSize *= 2;
            }
#ifdef DEBUG
            std::cout << "Got batch of size " << votes.size() << "\n";
#endif
            for (const auto& vote : votes) {

#ifdef DEBUG
                std::cout << "Process vote #" << vote.getId() << ": ";
                if (vote.getResult() == EVoteResult::Left) {
                    std::cout << vote.getLeftId() << " better than " << vote.getRightId();
                } else {
                    std::cout << vote.getRightId() << " better than " << vote.getLeftId();
                }
                std::cout << "...";
#endif

                DbRating leftRating(db), rightRating(db);
                EVoteResult resultValue = vote.getResult();

                leftRating.getById(vote.getLeftId());
                rightRating.getById(vote.getRightId());
                double addToLeft = calcEloAdd(leftRating.getRating(), leftRating.getGameCount(), rightRating.getRating(), resultValue == EVoteResult::Left);
                double addToRight = calcEloAdd(rightRating.getRating(), rightRating.getGameCount(), leftRating.getRating(), resultValue == EVoteResult::Right);

#ifdef DEBUG
                std::cout << "  " << leftRating.getRating() << " (" << addToLeft << ") vs " << rightRating.getRating() << "(" << addToRight << ")... ";
#endif

                leftRating.addToRating(addToLeft);
                rightRating.addToRating(addToRight);
                vote.resetPending();

#ifdef DEBUG
                std::cout << " Done." << std::endl;
#endif

             }
        }
    }

    return 0;
}

