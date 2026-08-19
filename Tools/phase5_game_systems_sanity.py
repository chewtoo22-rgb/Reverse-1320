#!/usr/bin/env python3
from dataclasses import dataclass, field

@dataclass
class Part:
    id: str
    slot: str
    price: int
    torque: float = 1.0
    mass_delta: float = 0.0
    grip: float = 1.0
    requires: list[str] = field(default_factory=list)
    conflicts: list[str] = field(default_factory=list)

@dataclass
class Profile:
    cash: int = 15000
    street_cred: int = 0
    installed: dict[str, Part] = field(default_factory=dict)
    ledger: list[tuple[str,int]] = field(default_factory=list)

    def tx(self, reason, amount):
        if amount < 0 and self.cash < -amount:
            return False
        self.cash += amount
        self.ledger.append((reason, amount))
        return True

    def install(self, part: Part):
        installed_ids = {p.id for p in self.installed.values()}
        if any(req not in installed_ids for req in part.requires):
            return False
        if any(conf in installed_ids for conf in part.conflicts):
            return False
        self.installed[part.slot] = part
        return True


def resolve_build(base_mass, parts):
    torque = 1.0
    grip = 1.0
    mass = base_mass
    for p in parts:
        torque *= p.torque
        grip *= p.grip
        mass += p.mass_delta
    return mass, torque, grip


def main():
    p = Profile()
    intake = Part('intake_stage1','intake',500,torque=1.04)
    turbo = Part('turbo_street','forced_induction',3500,torque=1.28,requires=['intake_stage1'])
    blower = Part('blower_street','forced_induction',3800,torque=1.25,conflicts=['turbo_street'])
    slick = Part('slick_275','tire',900,grip=1.18)
    weight = Part('rear_seat_delete','weight',250,mass_delta=-28)

    assert not p.install(turbo)
    assert p.tx('buy intake',-intake.price) and p.install(intake)
    assert p.tx('buy turbo',-turbo.price) and p.install(turbo)
    assert not p.install(blower)
    assert p.tx('buy slick',-slick.price) and p.install(slick)
    assert p.tx('buy weight',-weight.price) and p.install(weight)

    mass, torque, grip = resolve_build(1600,[intake,turbo,slick,weight])
    assert mass == 1572
    assert torque > 1.30
    assert grip > 1.10

    before = p.cash
    assert not p.tx('overspend',-(before+1))
    assert p.cash == before
    assert p.tx('race entry',-500)
    assert p.tx('race win',2200)
    p.street_cred += 125
    assert p.cash == 11550
    assert p.street_cred == 125

    # deterministic level gates
    tiers = [(1,0),(2,100),(3,300),(4,700)]
    level = max(level for level, cred in tiers if p.street_cred >= cred)
    assert level == 2

    print('Phase 5 game systems sanity: PASS')
    print(f'cash={p.cash} street_cred={p.street_cred} level={level}')
    print(f'mass={mass:.0f}kg torque_mult={torque:.3f} grip_mult={grip:.3f}')
    print(f'ledger_entries={len(p.ledger)}')

if __name__ == '__main__':
    main()
