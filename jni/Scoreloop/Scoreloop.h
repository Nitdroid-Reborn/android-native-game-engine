//
//  Scoreloop.h
//
//  Created by Mateusz P—¸rola on 07.08.2012.
//  Copyright (c) 2012 Vivid Games. All rights reserved.
//

#ifndef SCORELOOP_H
#define SCORELOOP_H

#include <map>
#include <string>

namespace Scoreloop
{
    typedef void (*RequestCallback)(int);
	typedef void (*TOSCallback)();
    
    /**
     * Defines error codes from Scoreloop
     */
    struct ErrorCode {
		enum Enum {
			UnknownError = -1,
			Ok = 0,
			RequestFailed = 1,
            
			EmailAlreadyTaken = 10,
			UsernameAlreadyTaken,
			EmailFormatInvalid,
			UsernameTooShort,
			UsernameFormatInvalid
		};
	};
    
    enum UserSearchCriterium {
        MatchPrefix = 0,
        ExactMatch,
        LikeMatch
    };
    
    enum ScoreSearchCriterium {
        Global = 0,
        UserCountry,
        Friends,
        TwentyfourHours
    };
    
    struct User;
    struct UserList;

    /**
     * Main Scoreloop client.
     * Initializes whole Scoreloop.
     */
    struct Client {
        /**
         * Initializes scoreloop, must be called before any other requests to scoreloop.
         * @param gameId specific game id from game's scoreloop panel
         * @param gameSecret specific game secret from game's scoreloop panel
         * @param tosCallback optional pointer to callback function, that will be called when user accept terms and conditions of Scoreloop.
         */
        static void initialize(const char* gameId, const char* gameSecret, TOSCallback tosCallback);
        
        /**
         * Deinitializes Scoreloop.
         */
        static void shutdown();
        
        /**
         * Checks if Scoreloop is initialized and if user accepted TOS.
         * @return true if user accepted TOS, false otherwise.
         */
        static bool isInitialized();
        
        /**
         * Returns user associated with current session.
         * @return current session user.
         */
        static User* getSessionUser();
    };
    
    

    
    /**
     * Scoreloop's user class.
     * To get or update user's data with Scoreloop server use UserController, all changes made to User object will not by automatically send to Scoreloop server.
     */
    struct User {
        /**
         * Returns unique user id, that is assigned by Scoreloop server.
         * @return string with user id
         */
        const char* getId();
            
        /**
         * Returns user login. Login can be changed by calling setLogin function.
         * @return string with user login
         */
        const char* getLogin();
        
        /**
         * Sets login for the user. If the login is used by other player, user update request will fail with error UsernameAlreadyTaken.
         * @param login new login for user
         */
        void setLogin(const char *login);
        
        /**
         * Returns user email. Email can be changed by calling setEngine function.
         * @return string with user email.
         */
        const char* getEmail();
        
        /**
         * Sets email for the user. If the email is used by other player, user update request will fail with error EmailAlreadyTaken(if user want merge two accounts it will receive email with this option).
         * @param email new email for user
         */
        void setEmail(const char *email);
        
        /**
         * Returns nationality of user. Nationality string is in ISO-3166 country-code format (plus optional 3166-2 region-code) e.g. "DE-BY".
         * @return string with ISO-3166 user nationality.
         */
        const char* getNationality();  
        
        /**
         * Sets nationality of user. Nationality string is in ISO-3166 country-code format (plus optional 3166-2 region-code) e.g. "DE-BY".
         * @param nationality string with ISO-3166 user nationality.
         */
        void setNationality(const char *nationality);
        
        
        /**
         * Returns user's skill level calculated by Scoreloop server.
         * @return user skill level
         */
        int getSkill();
        
        
        /**
         * Returns value for given key from user context(data associated with
         * user stored in Scoreloop cloud).
         * @param key key from context dictionary
         * @return value of given key
         */
        const char* getContextValue(const char *key);
        
        /**
         * Sets value of given key in user context(data associated with user stored in Scoreloop cloud).
         * @param key key from context dictionary
         * @param value value for given key
         */
        void setContextValue(const char *key, const char *value);
        
        
        /**
         * Cheks whether user context is loaded from server.
         */
        bool isContextLoaded();
        
        /**
         * Return list of user friends.
         */
        UserList* getFriendsList();
        
    private:
        static const char* getId(User *user);
        static const char* getLogin(User *user);
        static const char* getEmail(User *user);
        static void setLogin(User *user, const char *login);
        static void setEmail(User *user, const char *email);
        static const char* getNationality(User *user);
        static void setNationality(User *user, const char *nationality);
        static int getSkill(User *user);
        static const char* getContextValue(User *user, const char *key);
        static void setContextValue(User *user, const char *key, const char *value);
        static bool isContextLoaded(User* user);
        
        static UserList* getFriendsList(User *user);
    };
    
    

    /**
      * Users list.
      */
    struct UserList {
        /**
          * Returns count of users in list.
          * @return number of users in list.
          */
        unsigned int getCount();

        /**
          * Returns user with given index.
          * @param index index of user in list.
          * @return user with given index.
          */
        User* getUser(unsigned int index);
        
    private:
        static unsigned int getCount(UserList *userList);
        static User* getUser(UserList *userList, unsigned int index);
    };
    
    
    /**
      * Controller used to manage single user.
      * Downloads and uploads user data from/to Scoreloop server.
      */
    struct UserController {
        /**
          * Creates instance of controller.
          * @param callback pointer to callback function which will be called
          *        when controller finish request
          */
        static UserController* create(RequestCallback callback);

        /**
          * Releases controller. Controller must not be deleted, when no longer
          * required call release and assign 0 value.
          */
        void release();
        
        /**
          * Returns currently managed user.
          * @return user that was set by setUser function or default session user.
          */
        User* getUser();

        /**
          * Fetches user's data from Scoreloop server. This is asynchronous
          * request, will call callback function when finished.
          */
        void loadUser();

        /**
          * Uploads user's data to Scoreloop server. This is asynchronous
          * request, will call callback function when finished.
          */
        void submitUser();

        /**
          * Sets currently managed user.
          * @param user user to manage
          */
        void setUser(User* user);

        /**
          * Adds currently managed user as a friend of session user. This is
          * asynchronous request, will call callback function when finished.
          */
        void addAsFriend();

        /**
          * Removes currently managed user from friends of session user. This
          * is asynchronous request, will call callback function when finished.
          */
        void removeFromFriends();

        /**
          * Fetches currently managed user's friends from server. This is
          * asynchronous request, will call callback function when finished.
          */
        void requestFriends();

        /**
          * Fetches currently managed user's context(custom data associated
          * with user, stored in Scoreloop's cloud) from server. This is
          * asynchronous request, will call callback function when finished.
          */
        void requestContext();

        /**
          * Uploads currently managed user's context to server. This is
          * asynchronous request, will call callback function when finished.
          */
        void submitContext();
        
    private:
        static void release(UserController *controller);

        static User* getUser(UserController *controller);
        static void loadUser(UserController *controller);
        static void submitUser(UserController *controller);
        static void setUser(UserController *controller, User *user);
        static void addAsFriend(UserController *controller);
        static void removeFromFriends(UserController *controller);
        static void requestFriends(UserController *controller);
        static void requestContext(UserController *controller);
        static void submitContext(UserController *controller);
    };
    
    
    /**
      * Controller used to search users.
      */
    struct UsersController {
        /**
          * Creates instance of controller.
          * @param callback pointer to callback function which will be called
          *        when controller finish request.
          */
        static UsersController* create(RequestCallback callback);

        /**
          * Releases controller. Controller must not be deleted, when no longer
          * required call release and assign 0 value.
          */
        void release();
        

        /**
          * Search user/users based on their login. This is
          * asynchronous request, will call callback function when finished.
          * @param login exact login/prefix/regular expresion of login we want to find.
          * @param searchCriterium defines if login is exact login or prefix or regular expression.
          * @param limit limit of results that we want (if this limit is exceeded
          *        users list will be empty and isLimitExceeded function will return true).
          * @param global flag that defines if we want to search among all
          *        Scoreloop's users(true) or only among this who play this game(false)
          */
        void searchByLogin(const char* login, UserSearchCriterium searchCriterium, int limit, bool global);


        /**
          * Search user/users based on their email. This is
          * asynchronous request, will call callback function when finished.
          * @param email exact email/prefix/regular expresion of email we want to find.
          * @param searchCriterium defines if email is exact email or prefix or regular expression.
          * @param limit limit of results that we want (if this limit is exceeded
          *        users list will be empty and isLimitExceeded function will return true).
          * @param global flag that defines if we want to search among all
          *        Scoreloop's users(true) or only among this who play this game(false)
          */
        void searchByEmail(const char* email, UserSearchCriterium searchCriterium, int limit, bool global);
        

        /**
          * After finishing request started by calling searchByLogin or searchByEmail,
          * this function will indicate if search exceeded given limit.
          * @return true if limit was exceeded, false otherwise.
          */
        bool isLimitExceeded();


        /**
          * Returns search results.
          * @return list of found users that satisfy search criteria.
          */
        UserList* getUsers();
        
    private:
        static void release(UsersController*);
        static void searchByLogin(UsersController *controller, const char* login, UserSearchCriterium searchCriterium, int limit, bool global);
        
        static void searchByEmail(UsersController *controller, const char* email,UserSearchCriterium searchCriterium, int limit, bool global);
        
        static bool isLimitExceeded(UsersController *controller);
        static UserList* getUsers(UsersController *controller);
    };
    
    
    /**
      * Scoreloop's score.
      */
    struct Score {
        /**
          * Returns unique score id.
          * @return score id.
          */
        const char* getId();

        /**
          * Returns mode of score.
          * @return mode of score.
          */
        int getMode();

        /**
          * Returns level of score.
          * @return score's level.
          */
        int getLevel();

        /**
          * Returns major result.
          * @return major part of result.
          */
        float getResult();

        /**
          * Returns minor result.
          * @return minor part of result.
          */
        float getMinorResult();

        /**
          * Returns rank of score.
          * This will only have a meaningful value if the Score has been retrieved
          * from the server with a ScoresController.
          * @return score's rank.
          */
        int getRank();
        
        /**
         * Returns value for given key from score context(data associated with score stored in Scoreloop cloud).
         * @param key key from context dictionary
         * @return value of given key
         */
        const char* getContextValue(const char *key);

        /**
         * Sets value of given key in score context(data associated with score stored in Scoreloop cloud).
         * @param key key from context dictionary
         * @param value value for given key
         */
        void setContextValue(const char *key, const char *value);

        /**
          * Returns user to whom score belongs.
          * @return user that achieved that score.
          */
        User* getUser();
        
    private:
        static const char* getId(Score*);
        static int getMode(Score*);
        static int getLevel(Score*);
        static float getResult(Score*);
        static float getMinorResult(Score*);
        static int getRank(Score*);
        
        static const char* getContextValue(Score*, const char*);
        static void setContextValue(Score*, const char* key, const char*value);
        static User* getUser(Score*);
    };
    

    /**
      * Scores list.
      */
    struct ScoreList {
        /**
          * Returns count of scores in list.
          * @return number of scores in list.
          */
        unsigned int getCount();

        /**
          * Returns score with given index.
          * @param index index of score in list.
          * @return score with given index.
          */
        Score* getScore(unsigned int index);
    private:
        static unsigned int getCount(ScoreList*);
        static Score* getScore(ScoreList*, unsigned int index);
    };
    

    /**
      * Controller used to manage single score.
      */
    struct ScoreController {
        /**
          * Creates instance of controller.
          * @param callback pointer to callback function which will be called
          *        when controller finish request
          */
        static ScoreController* create(RequestCallback callback);

        /**
          * Releases controller. Controller must not be deleted, when no longer
          * required call release and assign 0 value.
          */
        void release();
        
        /**
          * Returns currently managed score.
          * @return score that was created by submitScore function.
          */
        Score* getScore();

        /**
          * Creates new score and sends it to Scoreloop's server.
          * This score is not associated with any challenge and
          * it will be assigned to session user.
          * It will be placed on leaderboard based on mode parameter.
          * This is asynchronous request, will call callback function when finished.
          * @param result major part of result.
          * @param minorResult minor part of result.
          * @param level level of game.
          * @param mode mode of game.
          * @param context custom data to store with score.
          */
        void submitScore(float result, float minorResult=0.0, int level=0, int mode=0, std::map<std::string, std::string>* context=0);
        
    private:
        static void release(ScoreController*);
        static Score* getScore(ScoreController*);
        void submitScore(ScoreController*, float result, float minorResult, int level, int mode, std::map<std::string, std::string>* context);
    };
    

    /**
      * Controller used to manage scores(leadeboards).
      */
    struct ScoresController {
        /**
          * Creates instance of controller.
          * @param callback pointer to callback function which will be called
          *        when controller finish request.
          */
        static ScoresController* create(RequestCallback callback);

        /**
          * Releases controller. Controller must not be deleted, when no longer
          * required call release and assign 0 value.
          */
        void release();
        
        /**
          * Sets mode for which we want to obtain scores from server.
          * @param mode mode of game.
          */
        void setMode(int mode);

        /**
          * Sets type of leaderboard that we want to obtain(global, 24 hour,
          * friends, user's country).
          * @param criterium type of leaderboard.
          */
        void setSearchCriterium(ScoreSearchCriterium criterium);
        
        /**
          * Loads list of scores from server.
          * This is asynchronous request, will call callback function when finished.
          * @param rangeStart start rank (counted from 0).
          * @param rangeLength num scores to load.
          */
        void loadRange(int rangeStart, int rangeLength);

        /**
          * Loads list of scores from server.
          * This is asynchronous request, will call callback function when finished.
          * @param rangeStart start rank (counted from 1).
          * @param rangeLength num scores to load.
          */
        void loadRangeFromRank(int rank, int rangeLength);

        /**
          * Loads list of scores from server, that includes the supplied score in the middle.
          * This is asynchronous request, will call callback function when finished.
          * @param score score in middle of list.
          * @param rangeLength num scores to load.
          */
        void loadRangeForScore(Score *score, int rangeLength);

        /**
          * Loads next range of scores. Should be only called when hasNextRange function returns true.
          * This is asynchronous request, will call callback function when finished.
          */
        void loadNextRange();

        /**
          * Loads previous range of scores. Should be only called when hasPrevRange function returns true.
          * This is asynchronous request, will call callback function when finished.
          */
        void loadPrevRange();
        
        /**
          * Indicates if additional scores can be load.
          * @return true if additional scores with lower rank can be load, false otherwise.
          */
        bool hasNextRange();

        /**
          * Indicates if additional scores can be load.
          * @return true if additional scores with higher rank can be load, false otherwise.
          */
        bool hasPrevRange();

        /**
          * Returns list of scores retrived from server.
          * @return loaded list of scores.
          */
        ScoreList* getScores();
    private:
        static void release(ScoresController*);
        static void setMode(ScoresController* controller, int mode);
        static void setSearchCriterium(ScoresController* controller, ScoreSearchCriterium criterium);
        
        static void loadRange(ScoresController* controller, int rangeStart, int rangeLength);
        static void loadRangeFromRank(ScoresController* controller, int rank, int rangeLength);
        static void loadRangeForScore(ScoresController* controller, Score* score, int rangeLength);
        static void loadNextRange(ScoresController*);
        static void loadPrevRange(ScoresController*);
        static bool hasNextRange(ScoresController*);
        static bool hasPrevRange(ScoresController*);
        static ScoreList* getScores(ScoresController*);
    };
    
    
    /**
      * Scoreloop's challenge.
      */
    struct Challenge {
        /**
          * Returns unique challenge id.
          * @return challenge id.
          */
        const char* getId();

        /**
          * Returns user that create challenge.
          * @return contender of challenge.
          */
        User* getContender();

        /**
          * Returns user that was challenged (null if this is open challenge).
          * @return contestant of challenge.
          */
        User* getContestant();

        /**
          * Returns mode of game.
          */
        int getMode();

        /**
          * Returns level of game.
          */
        int getLevel();
        

        /**
          * Sets mode of game.
          */
        void setMode(int mode);

        /**
          * Sets level of game.
          */
        void setLevel(int level);
        
        /**
         * Returns value for given key from challenge context(data associated with challenge stored in Scoreloop cloud).
         * @param key key from context dictionary
         * @return value of given key
         */
        const char* getContextValue(const char* key);

        /**
         * Sets value of given key in challenge context(data associated with challenge stored in Scoreloop cloud).
         * @param key key from context dictionary
         * @param value value for given key
         */
        void setContextValue(const char* key, const char* value);
        
        /**
          * Retuns winner of challenge or null if challenge is not yet finished.
          * @return user who win challenge.
          */
        User* getWinner();

        /**
          * Retuns loser of challenge or null if challenge is not yet finished.
          * @return user who lost challenge.
          */
        User* getLoser();

        /**
          * Retuns contender's score or null if contender don't send his score yet.
          * @return  Retuns contender's score or null if contender don't send his score yet.
          */
        Score* getContenderScore();

        /**
          * Retuns contestant's score or null if contestant don't send his score yet.
          * @return  Retuns contestant's score or null if contestant don't send his score yet.
          */
        Score* getContestantScore();

        /**
          * Indicates whether it is still possible to accept challenge.
          * @return false if challenge can still be accepted, true otherwise.
          */
        bool isDone();

        /**
          * Indicates whether challenge can be accepted by any user.
          * @return true if challenge can be accepted by any user, false otherwise.
          */
        bool isOpen();

        /**
          * Indicates whether challenge is created but contender don't send his score yet.
          * @return true if contender created challenge but don't send his score yet, false otherwise.
          */
        bool isCreated();

        /**
          * Indicates whether contender and contestant send their scores.
          * @return true if both players send their scores, false otherwise.
          */
        bool isComplete();

        /**
          * Indicates whether this challenge is assigned to given contestant.
          * It's negation of isOpen function.
          * @returns true if challenge is assigned to given contestant, false otherwise.
          */
        bool isAssigned();

        /**
          * Indicates whether challenge was cancelled by server.
          * @returns true if challenge was cancelled by server, false otherwise.
          */
        bool isCancelled();

        /**
          * Indicates whether challenge was rejected by contestant (this is only possible if this is not open challenge).
          * @return true if challenge was rejected, false otherwise.
          */
        bool isRejected();

        /**
          * Indicates whether challenge is accepted.
          * @return true if challenge is accepted, false otherwise.
          */
        bool isAccepted();

        /**
          * Indicates whether challenge is invalid (e.g contestant and contender are the same user).
          * @return true if challenge is invalid, false otherwise.
          */
        bool isInvalid(); 
        
        /**
          * Returns challenge stake;
          */
        int getStake();
        
    private:
        static const char* getId(Challenge* challenge);
        static User* getContender(Challenge* challenge);
        static User* getContestant(Challenge* challenge);
        static int getMode(Challenge* challenge);
        static int getLevel(Challenge* challenge);
        
        static void setMode(Challenge* challenge, int mode);
        static void setLevel(Challenge* challenge, int level);        
        
        static const char* getContextValue(Challenge* challenge, const char* key);
        static void setContextValue(Challenge* challenge, const char* key, const char* value);
        
        static User* getWinner(Challenge* challenge);
        static User* getLoser(Challenge* challenge);
        static Score* getContenderScore(Challenge* challenge);
        static Score* getContestantScore(Challenge* challenge);
        static bool isDone(Challenge* challenge);
        static bool isOpen(Challenge* challenge);
        static bool isCreated(Challenge* challenge);
        static bool isComplete(Challenge* challenge);
        static bool isAssigned(Challenge* challenge);
        static bool isCancelled(Challenge* challenge);
        static bool isRejected(Challenge* challenge);
        static bool isAccepted(Challenge* challenge);
        static bool isInvalid(Challenge* challenge);
        static int getStake(Challenge* challenge);
    };
    
    /**
      * List of challenges.
      */
    struct ChallengeList {
        /**
          * Returns number of challenges in list.
          * @return number of challenges.
          */
        unsigned int getCount();

        /**
          * Returns challenge with given index
          * @param index index of challenge.
          * @return challenge with given index.
          */
        Challenge* getChallenge(unsigned int index);
        
    private:
        static unsigned int getCount(ChallengeList* list);
        static Challenge* getChallenge(ChallengeList* list, unsigned int index);
    };
    
    
    /**
      * Controller used to manage single challenge.
      */
    struct ChallengeController {
        /**
          * Creates instance of controller.
          * @param callback pointer to callback function which will be called
          *        when controller finish request
          */
        static ChallengeController* create(RequestCallback callback);

        /**
          * Releases controller. Controller must not be deleted, when no longer
          * required call release and assign 0 value.
          */
        void release();

        /**
          * Sets currently managed challenge.
          * @param challenge challenge to manage.
          */
        void setChallenge(Challenge *challenge);

        /**
          * Returns currently managed challenge or if it was not set directly
          * by setChallenge function, returns active challenge from current session.
          * @return currently managed challenge.
          */
        Challenge* getChallenge();
        
        /**
          * Accepts challenge.
          * This is asynchronous request, will call callback function when finished.
          */
        void acceptChallenge();

        /**
          * Rejects challenge.
          * This is asynchronous request, will call callback function when finished.
          */
        void rejectChallenge();
             
        /**
          * Creates new challenge, setting current session user as contender.
          * @param contestand user to be challenged or null for open challenge.
          */
        void createChallenge(User* contestand);

        /**
          * Uploads challenge data to server.
          * This is asynchronous request, will call callback function when finished.
          */
        void submitChallenge();

        //TODO remove level and mode params, this values should be from challenge.
        /**
          * Sumbits score for challenge.
          * @param result major result part.
          * @param minorResult minor result part.
          * @param context additional data to be saved with score.
          */
        void submitChallengeScore(float result, float minorResult, int level, int mode, std::map<std::string, std::string>* context);

    private:
        static void release(ChallengeController* controller);
        static void setChallenge(ChallengeController* controller, Challenge*);
        static Challenge* getChallenge(ChallengeController* controller);
        
        static void acceptChallenge(ChallengeController* controller);
        static void rejectChallenge(ChallengeController* controller);
        static void submitChallengeScore(ChallengeController* controller, float result, float minorResult, int level, int mode, std::map<std::string, std::string>* context);
        static void submitChallenge(ChallengeController* controller);
        static void createChallenge(ChallengeController* controller, User* contestand);
    };
    
    /**
      * Controller used to manage list of challenges.
      */
    struct ChallengesController {
        /**
          * Creates instance of controller.
          * @param callback pointer to callback function which will be called
          *        when controller finish request.
          */
        static ChallengesController* create(RequestCallback callback);

        /**
          * Releases controller. Controller must not be deleted, when no longer
          * required call release and assign 0 value.
          */
        void release();
        
        /**
          * Returns mode of challenges to search.
          */
        int getMode();

        /**
          * Sets mode of challenges to search.
          */
        void setMode(int mode);

        /**
          * Request list of direct and open challenges for current session user.
          * This is asynchronous request, will call callback function when finished.
          */
        void requestChallenges();

        /**
          * Request history of challenges for current session user.
          * This is asynchronous request, will call callback function when finished.
          */
        void requestChallengesHistory();

        /**
          * Returns list of loaded challenges.
          * @param list of challenges loaded from server.
          */
        ChallengeList* getChallendirectges();

        
    private:
        static void release(ChallengesController* controller);
        
        static int getMode(ChallengesController* controller);
        static void setMode(ChallengesController* controller, int mode);
        static ChallengeList* getChallenges(ChallengesController* controller);
        static void requestChallenges(ChallengesController* controller);
        static void requestChallengesHistory(ChallengesController* controller);
    };

}
#endif
