# Generated by Django 3.0.6 on 2020-05-25 02:02

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('app', '0004_turn_playermoved'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='turn',
            name='playerMoved',
        ),
        migrations.AddField(
            model_name='turn',
            name='color_moved',
            field=models.CharField(default=None, max_length=6),
            preserve_default=False,
        ),
    ]
