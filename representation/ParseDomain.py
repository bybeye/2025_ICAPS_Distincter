import re
import logging

logger = logging.getLogger('Logger')


class ParseDomain:
    def __init__(self, input_pddl):
        self.pddl_content = self.remove_comments(input_pddl)
        self.pddl_content = self.pddl_content.replace('\t', '')
        self.pddl_content = self.pddl_content.lower()
        if self.check_valid(self.pddl_content) is False:
            raise ValueError('PDDL file error, stop working.')

        logger.info('PDDL is valid.')

    def remove_comments(self, input_pddl):
        return re.sub(';.*', '', input_pddl)

    def check_valid(self, input_pddl):
        parentheses_list = re.findall('(?:\(|\))', input_pddl)

        valid_num = 0
        for i, item in enumerate(parentheses_list):
            if item == '(':
                valid_num += 1
            elif item == ')':
                valid_num -= 1

            if valid_num < 0:
                logger.info('valid_num is less than 0')
                return False

            if valid_num == 0 and i != len(parentheses_list) - 1:
                logger.info(f'PDDL format error: early end parentheses, content: {parentheses_list} index: {i}')
                return False

        if valid_num != 0:
            logger.info(f'PDDL format error: parentheses format is wrong! content: {parentheses_list}')
            return False

        return True

    def parse_types(self):
        # support parse hierarchy types, but not record its type.
        types = []
        result = re.search(':types', self.pddl_content)

        if result is not None:
            sub_str = self.pddl_content[result.span()[1]:]
            sub_str = sub_str[:sub_str.index(')')].strip()
            # for spanner domain

            if '\n' in sub_str:
                row_list = sub_str.split('\n')
            else:
                row_list = [sub_str]

            for row in row_list:
                tmp_types = row.split(' ')
                for tmp_type in tmp_types:
                    if tmp_type in ('', '-'):
                        continue

                    if tmp_type not in types:
                        types.append(tmp_type)

            # while '' in types:
            #     types.remove('')
        else:
            types.append('object')
        return types

    def parse_constants(self):
        constants = {}
        result = re.search(":constants", self.pddl_content)

        if result is not None:
            sub_str = self.pddl_content[result.span()[1]:]
            sub_str = sub_str[:sub_str.index(')')].strip()
            # logger.info(sub_str)
            # constants = sub_str.split(" ")
            constants_str = sub_str.split('\n')
            for constant_str in constants_str:
                constant_str = constant_str.strip()
                constant_str_list = constant_str.split(' - ')
                if len(constant_str_list) != 2:
                    logger.info('constants format is wrong')
                else:
                    constant_type_name = constant_str_list[1].strip()
                    constant_objects = constant_str_list[0].strip().split(' ')
                    constants[constant_type_name] = constant_objects
        return constants

    def parse_predicates(self):
        predicates = {}
        result = re.search(':predicates', self.pddl_content)

        sub_str = self.pddl_content[result.span()[1]:]
        parentheses_list = re.finditer('(?:\(|\))', sub_str)
        # logger.info(sub_str)
        start = 0
        end = 0
        pair_depth = 1

        for i, item in enumerate(parentheses_list):
            if item.group() == '(':
                start = item.span()[0]
                pair_depth += 1
            elif item.group() == ')':
                pair_depth -= 1
                if pair_depth == 0:
                    # logger.info("Finish parsing goals!")
                    break
                if start != 0:
                    end = item.span()[1]
                    content = sub_str[start + 1:end - 1]

                    p_params = []
                    plist = content.split(' ')
                    p_name = plist[0]
                    i = 0
                    param_type_num = 0
                    while i < len(plist) - 1:
                        i += 1
                        if plist[i] in [' ', '']:
                            continue
                        # typing
                        if ' - ' in content:
                            if '?' in plist[i]:
                                param_type_num += 1
                            elif '-' == plist[i]:
                                i += 1
                                for _ in range(param_type_num):
                                    p_params.append(plist[i])
                                param_type_num = 0
                        else:
                            p_params.append(plist[i])

                    predicates[p_name] = p_params
                    start = 0
        return predicates
