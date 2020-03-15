<?php

namespace SixtyNine\Helper;

use Webmozart\Assert\Assert;

class Arrays
{
    public function setValue(array &$data, array $indexes, $value): void
    {
        $last = end($indexes);
        $parent = &$data;

        foreach ($indexes as $idx) {
            $isLastLevel = $idx === $last;

            if (!$isLastLevel) {
                if (!isset($parent[$idx])) {
                    $parent[$idx] = [];
                }

                $parent = &$parent[$idx];
                continue;
            }

            $parent[$idx] = $value;
        }
    }

    public function getValue(array $data, array $indexes)
    {
        $curItem = $data;
        foreach ($indexes as $idx) {
            if (!isset($curItem[$idx])) {
                return null;
            }
            $curItem = $curItem[$idx];
        }
        return $curItem;
    }

    public function setValueDotted(array &$data, string $key, $value): void
    {
        $this->setValue($data, explode('.', $key), $value);
    }

    public function getValueDotted(array $data, string $key)
    {
        return $this->getValue($data, explode('.', $key));
    }
}
