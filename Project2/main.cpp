#include <iostream>
#include <cassert>
#include "Sequence.h"

int main() {
    {
        Sequence ss;  // ItemType is std::string
        assert(ss.size() == 0 && ss.empty());  // Test size and empty work on an empty Sequence
        ss.insert(0, "aaa");
        assert(ss.size() == 1 && !ss.empty());  // Test size and empty work on a filled Sequence
        ss.insert(1, "bbb");
        assert(ss.size() == 2 && !ss.empty());  // Test size and empty work on a filled Sequence
        ss.insert(2, "ccc");
        assert(ss.size() == 3 && !ss.empty());  // Test size and empty work on a filled Sequence
        ItemType x = "xxx";
        assert(!ss.get(3, x) && x == "xxx");  // x is unchanged
        assert(ss.get(1, x) && x == "bbb");  // x is set to bbb
        assert(ss.insert(4, "ddd") == -1 && ss.insert(-1, "ddd"));  // adding item at invalid pos fails
        assert(ss.insert(0, "zzz") == 0 && ss.get(0, x) && x == "zzz");  // zzz is inserted at start
        assert(ss.insert(1, "yyy") == 1 && ss.get(1, x) && x == "yyy");  // yyy is inserted in middle
        assert(ss.size() == 5 && !ss.empty());  // Test size and empty work on a filled Sequence

        // Array currently: zzz, yyy, aaa, bbb, ccc
        assert(ss.erase(1) && ss.get(0, x) && x == "zzz" && ss.get(1, x) &&
               x == "aaa");  // Test that erase works in middle
        assert(ss.erase(0) && ss.get(0, x) && x == "aaa" && ss.get(1, x) &&
               x == "bbb");  // Test that erase works at start
        assert(ss.erase(2) && !ss.get(2, x) && ss.get(1, x) && x == "bbb");  // Test that erase works at end
        assert(!ss.erase(-1) && !ss.erase(2) && ss.get(0, x) && x == "aaa" && ss.get(1, x) &&
               x == "bbb");  // Test that fails with bad inputs and leaves everything unchanged
        assert(ss.size() == 2 && !ss.empty());  // Test size and empty work on a filled Sequence

        ss.insert(0, "fff");
        ss.insert(0, "ggg");
        ss.insert(0, "hhh");

        // List currently: hhh, ggg, fff, aaa, bbb
        assert(ss.set(0, "vvv") && ss.get(0, x) && x == "vvv");  // Test that set works
        assert(ss.set(2, "lll") && ss.get(2, x) && x == "lll");  // Test that set works
        assert(ss.set(4, "ooo") && ss.get(4, x) && x == "ooo");  // Test that set works
        assert(ss.get(0, x) && x == "vvv" && ss.get(1, x) && x == "ggg" && ss.get(2, x) && x == "lll" && ss.get(3, x) &&
               x == "aaa" && ss.get(4, x) && x == "ooo" && !ss.get(5, x) &&
               x == "ooo");  // Test that set didn't do anything funky to the list
        assert(!ss.set(-1, "ppp") && !ss.set(5, "ppp")); // Test that set fails when it should
        assert(ss.get(0, x) && x == "vvv" && ss.get(1, x) && x == "ggg" && ss.get(2, x) && x == "lll" &&
               ss.get(3, x) && x == "aaa" && ss.get(4, x) && x == "ooo" && !ss.get(5, x) &&
               x == "ooo");  // Test that set didn't do anything funky to the list


        Sequence st = Sequence();
        st.insert(0, "aaa");
        st.insert(0, "aaa");
        st.insert(0, "aaa");
        st.insert(0, "bbb");
        assert(st.remove("aaa") == 3 && st.size() == 1 && st.get(0, x) &&
               x == "bbb");  // test that removing from the tail works
        st.insert(1, "aaa");
        st.insert(2, "aaa");
        st.insert(3, "aaa");
        st.insert(4, "ccc");
        assert(st.remove("bbb") == 1 && st.size() == 4 && st.get(0, x) &&
               x == "aaa");  // test that removing from the front works

        Sequence su = Sequence();
        su.insert(0, "aaa");
        su.insert(1, "bbb");
        su.insert(2, "ccc");
        su.insert(3, "ddd");
        su.insert(4, "eee");
        assert(su.find("aaa") == 0 && su.find("bbb") == 1 && su.find("ccc") == 2 && su.find("ddd") == 3 &&
               su.find("eee") == 4 && su.find("fff") == -1);  // Test that find works correctly
        // su = aaa|bbb|ccc|ddd|eee

        Sequence sv = Sequence();
        sv.insert("aaa");
        sv.insert("bbb");
        sv.insert("ccc");
        sv.insert("ddd");
        sv.insert("eee");
        assert(sv.find("aaa") == 0 && sv.find("bbb") == 1 && sv.find("ccc") == 2 && sv.find("ddd") == 3 &&
               sv.find("eee") == 4 && sv.find("fff") == -1);  // Test that insert works correctly in the simple case
        sv.insert("aaa");
        sv.insert("111");
        sv.insert("zzz");
        assert(sv.find("111") == 0 && sv.find("aaa") == 1 && sv.find("bbb") == 3 && sv.find("ccc") == 4 &&
               sv.find("ddd") == 5 && sv.find("eee") == 6 &&
               sv.find("zzz") == 7); // Test that insert works in a more complex case
        // sv = 111|aaa|aaa|bbb|ccc|ddd|eee|zzz

        sv.swap(su);
        assert(sv.find("aaa") == 0 && sv.find("bbb") == 1 && sv.find("ccc") == 2 && sv.find("ddd") == 3 &&
               sv.find("eee") == 4 && sv.find("111") == -1 &&
               sv.find("zzz") == -1);  // Check that sv now contains the contents of su
        assert(su.find("111") == 0 && su.find("aaa") == 1 && su.find("bbb") == 3 && su.find("ccc") == 4 &&
               su.find("ddd") == 5 && su.find("eee") == 6 && su.find("zzz") == 7 &&
               su.size() == 8);  // Check the opposite

        Sequence sw = Sequence();
        sw.insert("aaa");
        sw.insert("bbb");
        sw.insert("ccc");
        sw.insert("ddd");
        sw.insert("eee");
        Sequence sx(sw);
        assert(sx.find("aaa") == 0 && sx.find("bbb") == 1 && sx.find("ccc") == 2 && sx.find("ddd") == 3 &&
               sx.find("eee") == 4 && sx.find("fff") == -1 && sx.size() == 5);  // Check a copy for something with data

        Sequence sy = Sequence();
        Sequence sz(sy);
        assert(sz.size() == 0 && !sz.get(0, x)); // Check copy of something with nothing in it

        su = sy;
        assert(su.size() == 0 && !su.get(0, x)); // Check that assignment works for empty stuff
        su = sw;
        assert(su.find("aaa") == 0 && su.find("bbb") == 1 && su.find("ccc") == 2 && su.find("ddd") == 3 &&
               su.find("eee") == 4 && su.find("fff") == -1 &&
               su.size() == 5);  // Check that assignment works for other things
    }
    ///////////////////
    // Test subsequence
    ///////////////////
    {
        Sequence sa = Sequence();
        sa.insert("aaa");
        sa.insert("aaa");
        sa.insert("aaa");
        sa.insert("aaa");
        sa.insert("aaa");
        sa.insert(0, "bbb");
        sa.insert(2, "bbb");
        // bbb|aaa|bbb|aaa|aaa|aaa|aaa

        // sb should fail
        Sequence sb = Sequence();
        sb.insert("bbb");
        sb.insert("bbb");
        sb.insert("bbb");
        assert(subsequence(sa, sb) == -1);

        // sc should succeed w/ 0
        Sequence sc = Sequence();
        sc.insert(0, "bbb");
        sc.insert(1, "aaa");
        assert(subsequence(sa, sc) == 0);

        // sd should succeed with 1
        Sequence sd = Sequence();
        sd.insert(0, "aaa");
        sd.insert(1, "bbb");
        assert(subsequence(sa, sd) == 1);

        // se should succeed with 2
        Sequence se = Sequence();
        se.insert(0, "bbb");
        se.insert(1, "aaa");
        se.insert(1, "aaa");
        assert(subsequence(sa, se) == 2);

        // sf should succeed with 3
        Sequence sf = Sequence();
        sf.insert("aaa");
        sf.insert("aaa");
        sf.insert("aaa");
        sf.insert("aaa");
        assert(subsequence(sa, sf) == 3);

        // sg should succeed with 3
        Sequence sg = Sequence();
        sg.insert("aaa");
        sg.insert("aaa");
        sg.insert("aaa");
        assert(subsequence(sa, sg) == 3);

        // sh should fail
        Sequence sh = Sequence();
        sh.insert("aaa");
        sh.insert("aaa");
        sh.insert("aaa");
        sh.insert("aaa");
        sh.insert("aaa");
        assert(subsequence(sa, sh) == -1);
    }
    {
        Sequence sa = Sequence();

        // sb should fail
        Sequence sb = Sequence();
        sb.insert("bbb");
        sb.insert("bbb");
        sb.insert("bbb");
        assert(subsequence(sa, sb) == -1);

    }
    //////////////////
    // Test interleave
    //////////////////
    {
        Sequence result = Sequence();

        Sequence sa = Sequence();
        sa.insert("aaa");
        sa.insert("bbb");
        sa.insert("ccc");
        sa.insert("ddd");
        sa.insert("eee");
        sa.insert(0, "fff");
        sa.insert(2, "ggg");

        Sequence sb = Sequence();
        sb.insert("zzz");
        sb.insert("xxx");
        sb.insert("yyy");

        // Check sizing and content of two with content, larger seq1
        interleave(sa, sb, result);
        assert(result.size() == 10);
        assert(result.find("fff") == 0 && result.find("xxx") == 1 && result.find("aaa") == 2);
        assert(result.find("yyy") == 3 && result.find("ggg") == 4 && result.find("zzz") == 5);
        assert(result.find("bbb") == 6 && result.find("ccc") == 7 && result.find("ddd") == 8);
        assert(result.find("eee") == 9);

        // Check sizing and content of two with content, larger seq2, non-zero content in result
        interleave(sb, sa, result);
        assert(result.size() == 10);
        assert(result.find("fff") == 1 && result.find("xxx") == 0 && result.find("aaa") == 3);
        assert(result.find("yyy") == 2 && result.find("ggg") == 5 && result.find("zzz") == 4);
        assert(result.find("bbb") == 6 && result.find("ccc") == 7 && result.find("ddd") == 8);
        assert(result.find("eee") == 9);

        // Check sizing and content for empty seq2 and non-zero content in result
        Sequence s0 = Sequence();
        interleave(sa, s0, result);
        assert(result.size() == 7);
        assert(result.find("fff") == 0 && result.find("aaa") == 1 && result.find("ggg") == 2);
        assert(result.find("bbb") == 3 && result.find("ccc") == 4 && result.find("ddd") == 5);
        assert(result.find("eee") == 6);

        // Check sizing and content for empty seq1 with empty result
        interleave(s0, sa, result);
        assert(result.size() == 7);
        assert(result.find("fff") == 0 && result.find("aaa") == 1 && result.find("ggg") == 2);
        assert(result.find("bbb") == 3 && result.find("ccc") == 4 && result.find("ddd") == 5);
        assert(result.find("eee") == 6);

        // Check sizing and content for empty both with non-empty result
        result.insert("fff");
        interleave(s0, s0, result);
        assert(result.size() == 0);

        // Check interleaving same item twice
        interleave(sb, sb, result);
        assert(result.size() == 6);
        ItemType itemType;
        assert(result.get(0, itemType) && itemType == "xxx" && result.get(1, itemType) && itemType == "xxx" &&
               result.get(2, itemType) && itemType == "yyy" && result.get(3, itemType) && itemType == "yyy" &&
               result.get(4, itemType) && itemType == "zzz" && result.get(5, itemType) && itemType == "zzz");

        // Test interleaving the same items as results
        interleave(sb, sb, sb);
        assert(sb.size() == 6);
        assert(result.get(0, itemType) && itemType == "xxx" && result.get(1, itemType) && itemType == "xxx" &&
               result.get(2, itemType) && itemType == "yyy" && result.get(3, itemType) && itemType == "yyy" &&
               result.get(4, itemType) && itemType == "zzz" && result.get(5, itemType) && itemType == "zzz");
        }


        std::cerr << "tests fin" << std::endl;
        return 0;
    }
