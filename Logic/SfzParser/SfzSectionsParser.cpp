//
// Created by Semyon Tykhonenko on 3/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "SfzSectionsParser.h"

#include <boost/spirit/include/qi.hpp>
#include <functional>
#include <boost/fusion/include/std_pair.hpp>

using namespace boost::spirit::qi;
using Iterator = std::string::const_iterator;
using RuleNoAttrs = rule<Iterator>;

BOOST_FUSION_ADAPT_STRUCT(
        SfzSection,
(std::string, name)
(SfzSection::Attrs, attrs)
)

struct Skipper : grammar<Iterator> {
    Skipper() : Skipper::base_type(skip) {
        single_line_comment = "//" >> *(char_ - eol) >> (eol|eoi);
        block_comment = ("/*" >> *(block_comment | char_ - "*/")) > "*/";
        skip = single_line_comment | block_comment | ascii::space;
    }

    rule<Iterator> skip;
    RuleNoAttrs block_comment;
    RuleNoAttrs single_line_comment;
};

std::vector<SfzSection> SfzSectionsParser::parse(const std::string& data) {
    std::vector<SfzSection> result;

    RuleNoAttrs block_comment;
    auto single_line_comment = "//" >> *(char_ - eol) >> (eol|eoi);
    block_comment = ("/*" >> *(block_comment | char_ - "*/")) > "*/";
    RuleNoAttrs skipper = single_line_comment | block_comment | ascii::space;

    rule<Iterator, std::string()> sectionTag = '<' >>+alpha>> '>';
    rule<Iterator, std::string()> key = +(char_('_')|alpha);
    rule<Iterator, std::string()> value = +(alnum | char_('_') | punct | char_('/'));
    rule<Iterator, std::pair<std::string, std::string>()> pair = key >> '=' >> value;

    rule<Iterator, SfzSection(), Skipper> section = sectionTag >> *pair;

    rule<Iterator, std::vector<SfzSection>(), Skipper> sfz = +section;

    phrase_parse(data.begin(), data.end(), sfz, Skipper(), result);

    return result;
}