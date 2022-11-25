# InductiveLogic
A one-player C++ console-based game, which is an adaptation of [Stanley Anderson's Jewels in the Sand](http://web.archive.org/web/20181120194631/http://pweb.jps.net:80/~sangreal/jits.htm).

**IMPORTANT**: The parser that evaluates your hypothesis string is extremely finicky. See Entering Rules below for instructions.

I use [Linenoise](https://github.com/antirez/linenoise), which for the average user just means you can press UP to show previous guesses and edit them to your desire.

# Note
This was made on Ubuntu (Linux). I make no guarantees that it will work on anything else and I have not bothered to try.

I abandoned this some time ago and only now decided to publish it. For some reason I had it in a debug state, so it is very much possible there are serious bugs.

## Running
`make run` should take care of everything for you. That being said, I'm new to this whole compatibility thing and so this has only been tested on Ubuntu.

## How to play
Your goal is to discover a secret pattern which some "words" (strings of length up to 6 or 8 using the letters `A, E, I, O, U`) follow and others do not.

When the program is launched, you are prompted with a difficulty level. Enter numbers from 1 to 5 to give a _range_ of levels for it to pick from (this is useful in case you don't want to know how hard the rules are). Higher-level rules are harder. There are 70 different level-1 rules alone.

You will then be given one random string that follows the rule, and another that does not. Each "turn", you make a **hypothesis** (guess) as to what you think the rule is, making sure that it fits the information you have. If your hypothesis is different from the actual rule, the game will find a negative word your hypothesis mislabeled as positive, and a positive word it mislabeled as negative. Then you keep making more hypotheses until you get it right. Your score is the number of revealed words minus two (for the initial ones); lower scores are better.

Sometimes, there may be, for instance, no false negatives (see example game below). In that case, you will only receive one counter-example. If your hypothesis classifies every string correctly, you win! (You don't have to get the exact wording. A winning guess is one that classifies every legal string correctly.)

### Example
Suppose you start with the following table:
```
Positive | Negative
-------------------
     AIO | OO
```
 `AIO` is a positive and `OO` is negative. There are many possibilities for what the rule could be ("exactly 1 O", "more A than E", etc). Suppose we guess `at least 1 A`. That is, predictably, not the correct rule. The game then says `IEEE` is really a positive (despite NOT having any `A`s), and `UIAUI` is a negative (even though it has an `A`).

Now we try again. Both positive words contain the letter `I`, but one of the negative words does as well. However, the negative word contains _two_ `I`'s, so maybe the rule is `exactly 1 I`.

This time, the game spits out `IIOAIE` as a positive, but gives no negative word. This is interesting! It means any word with exactly 1 `I` must be positive, but there are other positives too. In this case, if you are really clever, you may notice that the counterexample has three `I`'s, while the one negative word has two. What separates 1 and 3 from 0 and 2? Of course: `an odd number of I` (no plural, because my parser does not recognize them... it would think the S was part of the pattern, with unpredictable results). This is indeed the rule, and you win.

## Entering Rules
As I stated previously, the parser is a bit finicky about what it accepts. Case is ignored completely, so `aT LeaST 1 u` works just as well as `at least 1 U`.

#### Rules
The two basic types of rules you can implement are:
- Counting rules, like `at least 2 E`. This must start with `at least`, `at most`, `exactly`, `an odd number of`, or `an even number of`. After this comes a number (unless you used odd or even), optionally the word "of" (`at least 2 E` and `at least 2 of E` are both allowed) and then the pattern.
	- Since "at least 1" and "exactly 0" are common, I allow `some` and `any` (possibly followed by `of`) as shortcuts for the former, and `no` and `none of` as shortcuts for the latter.
- Rules comparing two patterns, like `more E than A`. You can also use `less` and `fewer` for the opposite comparison. I think `as many of E as A` works for equality, but the actual rule never uses it.

You can also form compound rules with `and` and `or`: `at least 2 E and at most 2 I` works. Parentheses are required if you mix "or" with "and": `(at least 2 E and at most 2 I) or more O than A`, but `at least 1 A or at least 1 E or at least 1 I` works as is.

#### Patterns
You can have rules count strings of multiple letters, so `at least 2 OI` is a valid rule.

In addition, there are three different kinds of "wildcards":
- `?` can represent any single letter.
- `*` can represent any series of letters, including nothing at all. (This allows `some A*U` to represent the concept "contains an A that is before a U", so `AUI` would be positive, as would `AEEU`.)
- Digits represent single letters, but the same digit must represent the same letter, AND different digits must be different letters. For example, `121342` would match `AOAUEO`. The only practical use of this right now is that `11` stands for a double letter and `123` is a series of three different letters.

For example, `no A??E` means "there does not exist an A that is exactly three letters before an E".

The characters `s` and `f` mean "start" and "finish" (`e` for `end` is already used...) For example, `no sI` means "does not start with `I`".

#### Other information

I personally don't mind if you peek at the list of possible rules while playing (see RULES.txt); level 1 alone can generate 70 different rules.

Levels 4 and 5 make compound rules, which can sometimes get a bit... weird. The 8-character limit can be important (there are many rules where a winning guess exists that would get a 9-character string wrong).

There are some potential workarounds to various ideas:
- "every A is followed by E": You could do this as `no AA and no AI and no AO and no AU`, or `not (more A? than AE)`.
- "at least 1 A followed by either E or I": `some AE or some AI`. However, this will not work for "at least 2 total of them".

It's possible that higher-level rules randomly invent something that looks impossible to represent. Fortunately, you can take advantage of the limited size of strings in the game. If your guess is equivalent for up to 8 characters, you win, even if the rules would differ on some 9-character string.

However, if you still find it impossible to represent your idea, it's okay because the game only generates rules that can be entered, so the idea must be wrong (or equivalent to something else).
