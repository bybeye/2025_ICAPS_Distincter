import re
import logging

logger = logging.getLogger('Logger')


class ParseProblem:
    def __init__(self, input_pddl):
        self.pddl_content = self.remove_comments(input_pddl)
        self.pddl_content = self.pddl_content.replace('\t', ' ')
        self.pddl_content = self.pddl_content.lower()
        self.init_achieved = False

        if self.check_valid(self.pddl_content) is False:
            raise ValueError('PDDL file error, stop working.')

        # logger.info("PDDL is valid.")

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

            if valid_num < 0 or (valid_num == 0 and i != len(parentheses_list) - 1):
                logger.info(f'PDDL format error: early end parentheses, content: {parentheses_list}, index: {i}')
                return False

        if valid_num != 0:
            logger.info(f'PDDL format error: parentheses format is wrong! content: {parentheses_list}')
            return False

        return True

    def parse_goals(self):
        # logger.info("Begin parse_goals")
        goals = []
        result = re.search(':goal', self.pddl_content)
        if result is not None:
            sub_str = self.pddl_content[result.span()[1]:-1]
            parentheses_list = re.finditer('(?:\(|\))', sub_str)
            # logger.info(sub_str)
            start = 0
            end = 0
            pair_depth = 1
            for item in parentheses_list:
                # logger.info("index:", i, item.group())
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
                        # logger.info(sub_str[start:end])
                        goals.append(sub_str[start + 1:end - 1].strip())
                        start = 0
        return goals

    def parse_inits(self):
        # logger.info("Begin parse_inits")
        inits = []
        result = re.search(':init', self.pddl_content)
        if result is not None:
            sub_str = self.pddl_content[result.span()[1]:]
            parentheses_list = re.finditer('(?:\(|\))', sub_str)
            # logger.info(sub_str)
            start = 0
            end = 0
            pair_depth = 1
            for _, item in enumerate(parentheses_list):
                # logger.info("index:", i, item.group())

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
                        # logger.info(sub_str[start:end])
                        inits.append(sub_str[start + 1:end - 1].strip())
                        start = 0
        return inits

    def parse_objects(self):
        objects = {}
        result = re.search(':objects', self.pddl_content)

        if result is not None:
            sub_str = self.pddl_content[result.span()[1]:]
            sub_str = sub_str[:sub_str.index(')')].strip()
            # logger.info(sub_str)

            types_str = sub_str.split('\n')
            temp_str = ''

            for _, type_str in enumerate(types_str):
                if '- ' in type_str:
                    if temp_str != '':
                        type_str = temp_str + type_str
                        temp_str = ''
                    type_str = type_str.strip()
                    type_str_list = type_str.split(' - ')
                    if len(type_str_list) != 2:
                        logger.info('objects format is wrong')
                    else:
                        type_name = type_str_list[1].strip()
                        type_objects = type_str_list[0].strip().split(' ')
                        while '' in type_objects:
                            type_objects.remove('')
                        objects[type_name] = type_objects
                        logger.info(type_name, type_objects)
                        # print(type_name, type_objects)
                else:
                    temp_str += type_str

            if temp_str != '':
                type_objects = temp_str.strip().split(' ')
                while '' in type_objects:
                    type_objects.remove('')
                objects['object'] = type_objects
                logger.info(f'No type in objects, object: {type_objects}')

        return objects
